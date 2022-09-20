#include "includes.hpp"

void Server::handle_connection(int clientSocket, id_server_type server_id){
	std::cout << "server id : " << server_id << std::endl;
	treat_request(clientSocket);

	check_host(server_id);

	std::string response = HGen.getStr();
	send(clientSocket, response.c_str(), response.size(), SOCK_DGRAM);
}
