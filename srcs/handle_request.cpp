#include "includes.hpp"

void Server::handle_connection(int clientSocket, struct serverInfo serverInfo){
	treat_request(clientSocket);

	parseG.location["/"].root = "html_files/";// tmp attend pour parsing autofill && add setter to add / back if needed
	parseG.path_e_400 = fileLocation("error_files/error_400.html");
	parseG.path_e_404 = fileLocation("error_files/error_404.html");
	parseG.index.push_back("index.html");

	check_host();

	std::string response = HGen.getStr();
	send(clientSocket, response.c_str(), response.size(), SOCK_DGRAM);
}
