#include "includes.hpp"

void Server::handle_connection(int clientSocket, id_server_type server_id){
	std::cout << "server id : " << server_id << std::endl;

	int method;
	method = treat_request(clientSocket);
	
	if ( method == GET ){
		std::string to_send;
		std::cout << CYAN << "METHOD = GET " << WHITE << std::endl;

		try{
			to_send = treat_GET_request("index.html"/*wait parsing*/, server_id);
			send_200(to_send, server_id);//! do the file dynamic
		}
		catch (const Error_page& page){
			std::string err = page.what();
			if (err == "400")
				send_400(server_id);
			else if (err == "404")
				send_404(server_id);
		}
	}
	else if ( method == POST )
		treat_POST_request(_header);
	else if ( method == DELETE )
		std::cout << CYAN << "METHOD = DELETE " << WHITE << std::endl;
	else
	{
		std::cout << CYAN << "SOMETHING WENT WRONG WITH THE REQUEST " << WHITE << std::endl;
		exit(0);
	}



	std::string response = HGen.getStr();
	send(clientSocket, response.c_str(), response.size(), SOCK_DGRAM);
}
