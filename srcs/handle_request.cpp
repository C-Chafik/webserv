#include "includes.hpp"

void Server::handle_connection(int clientSocket, struct serverInfo serverInfo){
	treat_request(clientSocket);

	//traitement de la request

	//	tmp attend pour parsing autofill
	parseG.location["html_files"].root = "/var/www/html/";

	std::cout << fileLocation("html_files/test.html") << std::endl;
	std::string fileSTR = fileToString("html_files/test.html", serverInfo);


	HGen.setStatus("200 OK");
	HGen.setType("text/html");
	HGen.setContentString(fileSTR);
	HGen.processResponse();

	std::string response = HGen.getStr();
	send(clientSocket, response.c_str(), response.size(), SOCK_DGRAM);
}
