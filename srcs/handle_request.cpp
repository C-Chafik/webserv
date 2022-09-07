#include "includes.hpp"

void Server::handle_connection(int clientSocket, struct serverInfo serverInfo){
	treat_request(clientSocket);

	parseG.location["html_files"].root = "/var/www/html/";// tmp attend pour parsing autofill

	// std::cout << fileLocation("html_files/test.html") << std::endl;

	// HGen.setStatus("200 OK");
	// HGen.setType("text/html");
	// HGen.setContentString(fileSTR);
	// HGen.processResponse();

	/* 
	TODO parse Host in request header end split ip and port if host is an ip
	*/
	parseG.ip_address = inet_addr("127.0.0.1");
	parseG.server_names.push_back("localhost");
	short send_type = host(inet_addr("127.0.0.1"), "localhost");

	switch (send_type)
	{
		case D_200:
			send_200("html_files/test.html");
			break;
		
		case D_400:
			send_400();
			break;

		default:
			break;
	}
	

	std::string response = HGen.getStr();
	send(clientSocket, response.c_str(), response.size(), SOCK_DGRAM);
}
