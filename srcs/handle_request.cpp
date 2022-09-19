#include "includes.hpp"

void Server::handle_connection(int clientSocket){
	treat_request(clientSocket);

	check_host();

	std::string response = HGen.getStr();
	send(clientSocket, response.c_str(), response.size(), SOCK_DGRAM);
}
