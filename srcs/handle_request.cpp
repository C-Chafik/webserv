#include "includes.hpp"

void Server::handle_connection(int clientSocket, struct serverInfo serverInfo){
	treat_request(clientSocket);

	parseG.location["html_files"].root = "/var/www/html/";// tmp attend pour parsing autofill
	parseG.path_e_400 = "html_files/error_400.html";
	parseG.path_e_404 = "html_files/error_404.html";

	check_host();

	std::string response = HGen.getStr();
	send(clientSocket, response.c_str(), response.size(), SOCK_DGRAM);
}
