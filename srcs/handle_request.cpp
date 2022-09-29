#include "includes.hpp"

bool Server::handle_connection(int clientSocket, id_server_type server_id)
{
	std::cout << "server id : " << server_id << std::endl;

	if (request.find(server_id) == request.end())
		request.insert(std::make_pair(server_id, receive_request(clientSocket)));

	if ( request[server_id].method == POST )
	{
		if ( request[server_id].full == false )
		{
			receive_request_body(request[server_id], clientSocket);
			if ( request[server_id].full == false )
				return true;//keep alive
		}
	}

	int method = request[server_id].method;

	if ( method == GET ){
		std::string to_send;
		std::cout << CYAN << "METHOD = GET " << WHITE << std::endl;

		try{
			to_send = treat_GET_request(request[server_id], server_id);
			send_200(to_send, server_id);//! do the file dynamic
		}
		catch (const Error_page& page){
			std::string err = page.what();
			if (err == "400")
				send_400(server_id);
			else if (err == "404")
				send_404(server_id);
		}
		request.erase(server_id);
	}
	else if ( method == POST )
	{
		std::cout << "TREATING POST HEADER" << std::endl;
		treat_POST_request(request[server_id].header);
		request.erase(server_id);
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
