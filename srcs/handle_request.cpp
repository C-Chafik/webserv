#include "includes.hpp"

void Server::handle_connection(int clientSocket, id_server_type server_id){
	std::cout << "server id : " << server_id << std::endl;

	int method;

	method = treat_request(clientSocket);

	if ( method == GET )
		std::cout << CYAN << "METHOD = GET " << WHITE << std::endl;
	else if ( method == POST )
		treat_POST_request(_header);
	else if ( method == DELETE )
		std::cout << CYAN << "METHOD = DELETE " << WHITE << std::endl;
	else
	{
		std::cout << CYAN << "SOMETHING WENT WRONG WITH THE REQUEST " << WHITE << std::endl;
		exit(0);
	}


	check_host(server_id);

	std::string response = HGen.getStr();
	send(clientSocket, response.c_str(), response.size(), SOCK_DGRAM);
}
