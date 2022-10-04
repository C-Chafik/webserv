#include "includes.hpp"

void Server::send_200(std::string file){
	HGen.clear();
	std::string fileSTR = fileToString(file);

	HGen.setStatus("200 OK");
	HGen.setType("text/html");
	HGen.setContentString(fileSTR);
	HGen.processResponse();
}

void Server::send_400(id_server_type serverNb){
	HGen.clear();
	std::string fileSTR = fileToString(findPathError(serverNb, 400), true);

	HGen.setStatus("400 Bad Request");
	HGen.setType("text/html");
	HGen.setContentString(fileSTR);
	HGen.processResponse();
}

void Server::send_404(id_server_type serverNb){
	HGen.clear();
	std::string fileSTR = fileToString(findPathError(serverNb, 404), true);

	HGen.setStatus("404 Not Found");
	HGen.setType("text/html");
	HGen.setContentString(fileSTR);
	HGen.processResponse();
}

void Server::send_301(std::string location){
	HGen.clear();

	HGen.setStatus("301 Moved Permanently");
	HGen.setLocation(location);
	HGen.processResponse();
}

void Server::send_cgi(std::string data){
	HGen.clear();

	HGen.setStatus("200 OK");
	HGen.setType("text/html");
	HGen.setContentString(data);
	HGen.processResponse();
}