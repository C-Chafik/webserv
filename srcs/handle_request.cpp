#include "includes.hpp"

bool		Server::check_request_validity( struct header & header, id_server_type server_id )
{
	if ( confs[server_id].locations.find(header.path) == confs[server_id].locations.end() )
	{
		std::cout << "THIS PATH DONT EXIST " << std::endl;
		send_404(server_id);
		return false;
	}
	else if ( header.content_length > confs[server_id].locations[header.path].body_max_size && header.method == POST )
	{
		std::cout << "BODY MAX SIZE IS EXCEEDED !" << std::endl;
		send_400(server_id);
		return false;
	}
	else if ( header.method == GET )
	{
		std::cout << "METHOD == GET " << std::endl;
		if (  confs[server_id].locations[header.path].GET == false )
		{
			send_400(server_id);
			return false;
		}
		std::cout << "METHOD GET IS AUTORISED " << std::endl;
	}
	else if ( header.method == POST )
	{
		std::cout << "METHOD == POST " << std::endl;
		if (  confs[server_id].locations[header.path].POST == false )
		{
			send_400(server_id);
			return false;
		}
		std::cout << "METHOD POST IS AUTORISED " << std::endl;
	}
	else if ( header.method == DELETE )
	{
		std::cout << "METHOD == DELETE " << std::endl;
		if (  confs[server_id].locations[header.path].DELETE == false )
		{
			send_400(server_id);
			return false;
		}
		std::cout << "METHOD DELETE IS AUTORISED " << std::endl;
	}
	return true;
}

bool Server::handle_connection(int clientSocket, id_server_type server_id)
{
	std::cout << "server id : " << server_id << std::endl;

	if ( all_request.find(server_id) == all_request.end() )
	{
		Request request;
		all_request.insert(std::make_pair(server_id, request));
	}

	all_request[server_id].receive_request(clientSocket);
	
	int method = all_request[server_id].get_header().method;

	if ( method == GET )
	{
		std::string to_send;
		try{
			to_send = treat_GET_request(all_request[server_id].get_header(), server_id, clientSocket);
			if (!to_send.empty())
				send_200(to_send);
		}
		catch (const Error_page& page){
			std::string err = page.what();
			if (err == "400")
				send_400(server_id);
			else if (err == "404")
			{
				std::cout << "ERROR 404 WTF" << std::endl;
				send_404(server_id);
			}
			else{
				check_server_name(all_request[server_id].get_header(), server_id);
				send_301(confs[server_id].locations[err].http_redirection.second);
			}
		}
		std::cout << CYAN << "END METHOD = GET " << WHITE << std::endl;
		//! deleting the tmp file
		remove(all_request[server_id].get_file_path().c_str());
		std::cout <<  strerror(errno) << std::endl;
		all_request.erase(server_id);
	}
	else if ( method == POST )
	{
		if ( check_request_validity(all_request[server_id].get_header(), server_id ) == false )
		{
			std::string response = HGen.getStr();
			send(clientSocket, response.c_str(), response.size(), SOCK_DGRAM);
			return false;
		}

		if ( all_request[server_id].is_full() == false )
			return true;
		
		treat_POST_request(all_request[server_id].get_header(), all_request[server_id].get_body(), all_request[server_id].get_file_path(), server_id);
		all_request.erase(server_id);
		std::cout << CYAN << "END METHOD = POST " << WHITE << std::endl;
	}
	else if ( method == DELETE )
		std::cout << CYAN << "METHOD = DELETE " << WHITE << std::endl;


	std::string response = HGen.getStr();
	send(clientSocket, response.c_str(), response.size(), SOCK_DGRAM);

	return all_request[server_id].get_header().keep_alive;//add function to keep alive
}
