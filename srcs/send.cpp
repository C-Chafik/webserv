#include "includes.hpp"

void Server::send_200(std::string file){
	std::string fileSTR = fileToString(fileLocation(file));

	HGen.setStatus("200 OK");
	HGen.setType("text/html");
	HGen.setContentString(fileSTR);
	HGen.processResponse();
}

void Server::send_400(){
	std::string fileSTR = fileToString(parseG.path_e_400, true);//!raw until parsing done (path of error 400)

	HGen.setStatus("400 Bad Request");
	HGen.setType("text/html");
	HGen.setContentString(fileSTR);
	HGen.processResponse();
}

void Server::send_404(){
	std::string fileSTR = fileToString(parseG.path_e_404, true);//!raw until parsing done (path of error 404)

	HGen.setStatus("404 Not Found");
	HGen.setType("text/html");
	HGen.setContentString(fileSTR);
	HGen.processResponse();
}
