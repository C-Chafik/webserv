#include "includes.hpp"

bool		Server::check_GET_request_validity( struct header & header, id_server_type server_id )
{
	std::string location_name = retrieve_location_name(header.path, server_id);
	
	if ( confs[server_id].locations.find(location_name) != confs[server_id].locations.end() )
	{
		// std::cout << "THIS PATH EXIST " << std::endl;
		// std::cout << "FOR HEADER PATH >> " << location_name << std::endl;
		if ( confs[server_id].locations[location_name].GET == false )
		{
			// std::cout << " METHOD GET IS NOT AUTORISED HERE ! " << std::endl;
			send_405(server_id);
			return false;
		}
	}
	return true;
}


bool		Server::check_DELETE_request_validity( struct header & header, id_server_type server_id )
{
	std::string location_name = retrieve_location_name(header.path, server_id);
		
	if ( confs[server_id].locations.find(location_name) != confs[server_id].locations.end() )
	{
		// std::cout << "THIS PATH EXIST " << std::endl;
		// std::cout << "FOR HEADER PATH >> " << location_name << std::endl;
		if ( confs[server_id].locations[location_name].DELETE == false )
		{
			// std::cout << " METHOD DELETE IS NOT AUTORISED HERE ! " << std::endl;
			send_405(server_id);
			return false;
		}
	}
	return true;
}

bool		Server::check_POST_request_validity( struct header & header, id_server_type server_id )
{
	std::string location_name = retrieve_location_name(header.path, server_id);
	
	if ( confs[server_id].locations.find(location_name) != confs[server_id].locations.end() )
	{
	// 	std::cout << "THIS PATH EXIST " << std::endl;
	// 	std::cout << "FOR HEADER PATH >> " << location_name << std::endl;
		if ( header.content_length > confs[server_id].body_max_size && header.method == POST )
		{
			// std::cout << header.content_length << " > " <<  confs[server_id].body_max_size << std::endl;
			// std::cout << "BODY MAX SIZE IS EXCEEDED !" << std::endl;
			send_413(server_id);
			return false;
		}
		if ( confs[server_id].locations[location_name].POST == false )
		{
			// std::cout << " METHOD POST IS NOT AUTORISED HERE ! " << std::endl;
			send_405(server_id);
			return false;
		}
	}
	return true;
}

void Server::Get(int clientSocket, id_server_type server_id){
	std::string to_send;
		try{
			to_send = treat_GET_request(all_request[server_id].get_header(), server_id, clientSocket);
			if (!to_send.empty()){
				send_responses(to_send);
			}
		}
		catch (const Error_page& page){
			std::string err = page.what();
			if (err == "400")
				send_400(server_id);
			else if (err == "404")
				send_404(server_id);
			else{
				check_server_name(all_request[server_id].get_header(), server_id);
				send_301(confs[server_id].locations[err].http_redirection.second);
			}
		}
		// std::cout << CYAN << "END METHOD = GET " << WHITE << std::endl;
		//! deleting the tmp file
		remove(all_request[server_id].get_file_path().c_str());
		all_request.erase(server_id);
}

bool Server::handle_connection(int clientSocket, id_server_type server_id)
{
	// std::cout << "server id : " << server_id << std::endl;

	if ( all_request.find(server_id) == all_request.end() )
	{
		Request request;
		all_request.insert(std::make_pair(server_id, request));
	}
	all_request[server_id].read_client(clientSocket);

	int method = all_request[server_id].get_header().method;

	if ( method == GET )
	{
		if ( check_GET_request_validity(all_request[server_id].get_header(), server_id ) == false )
		{
			std::string response = HGen.getStr();
			send(clientSocket, response.c_str(), response.size(), SOCK_DGRAM);
			remove(all_request[server_id].get_file_path().c_str());
			return false;
		}
		Get(clientSocket, server_id);
	}

	else if ( method == POST )
	{
		if ( check_POST_request_validity(all_request[server_id].get_header(), server_id ) == false )
		{
			std::string response = HGen.getStr();
			send(clientSocket, response.c_str(), response.size(), SOCK_DGRAM);
			remove(all_request[server_id].get_file_path().c_str());
			return false;
		}

		if ( all_request[server_id].is_full() == false )
			return true;
		treat_POST_request(all_request[server_id].get_header(), all_request[server_id].get_body(), all_request[server_id].get_file_path(), server_id);
		all_request.erase(server_id);
		// std::cout << CYAN << "END METHOD = POST " << WHITE << std::endl;
	}
	else if ( method == DELETE )
	{
		if ( check_DELETE_request_validity(all_request[server_id].get_header(), server_id ) == false )
		{
			std::string response = HGen.getStr();
			send(clientSocket, response.c_str(), response.size(), SOCK_DGRAM);
			remove(all_request[server_id].get_file_path().c_str());
			return false;
		}

		treat_DELETE_request(all_request[server_id].get_header(), server_id);
		remove(all_request[server_id].get_file_path().c_str());
		send_202();
		all_request.erase(server_id);
		// std::cout << CYAN << "END METHOD = DELETE " << WHITE << std::endl;
	}
	else
		send_501(server_id);


	std::string response = HGen.getStr();
	send(clientSocket, response.c_str(), response.size(), SOCK_DGRAM);

	// std::cout << "REPONSE [" << response << "]" << std::endl;

	return all_request[server_id].get_header().keep_alive;
}
