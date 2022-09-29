#include "includes.hpp"

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
		std::cout << CYAN << "METHOD = GET " << WHITE << std::endl;

		try{
			to_send = treat_GET_request(all_request[server_id].get_header(), server_id);
			send_200(to_send, server_id);//! do the file dynamic
		}
		catch (const Error_page& page){
			std::string err = page.what();
			if (err == "400")
				send_400(server_id);
			else if (err == "404")
				send_404(server_id);
		}
		all_request.erase(server_id);
	}
	else if ( method == POST )
	{
		std::cout << CYAN << "METHOD = POST " << WHITE << std::endl;
		if ( all_request[server_id].is_full() == false )
			return true;
		treat_POST_request(all_request[server_id].get_header(), all_request[server_id].get_body());
		all_request.erase(server_id);
	}
	else if ( method == DELETE )
		std::cout << CYAN << "METHOD = DELETE " << WHITE << std::endl;
	else
	{
		std::cout << CYAN << "SOMETHING WENT WRONG WITH THE REQUEST " << WHITE << std::endl;
		exit(0);
	}



	std::string response = HGen.getStr();
	send(clientSocket, response.c_str(), response.size(), SOCK_DGRAM);

	return false;//add function to keep alive
}
