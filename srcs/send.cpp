#include "includes.hpp"

void Server::send_200(std::string file){
	std::string fileSTR = fileToString(file, serverInfo);

	HGen.setStatus("200 OK");
	HGen.setType("text/html");
	HGen.setContentString(fileSTR);
	HGen.processResponse();
}

void Server::send_400(){
	std::string fileSTR = fileToString("html_files/error_400.html", serverInfo);

	HGen.setStatus("400 Bad Request");
	HGen.setType("text/html");
	HGen.setContentString(fileSTR);
	HGen.processResponse();
}