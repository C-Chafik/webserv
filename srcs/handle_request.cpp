#include "includes.hpp"

void Server::handle_connection(int clientSocket, struct serverInfo serverInfo){
	treat_request(clientSocket);

	parseG.location["html_files"].root = "/var/www/html/";// tmp attend pour parsing autofill

	check_host();

	std::string response = HGen.getStr();
	send(clientSocket, response.c_str(), response.size(), SOCK_DGRAM);
}
