#include "includes.hpp"

/**
 * create response with a 200 Header
 * Do the routing of the path
 * return 404 if not found
 */
void Server::send_200(std::string file, id_server_type serverNb){
	HGen.clear();
	std::string fileSTR = fileToString(serverNb, file);

	HGen.setStatus("200 OK");
	HGen.setType("text/html");
	HGen.setContentString(fileSTR);
	HGen.processResponse();
}

void Server::send_400(id_server_type serverNb){
	HGen.clear();
	std::string fileSTR = fileToString(serverNb, findPathError(serverNb, 400), true);//!raw until parsing done (path of error 400)

	HGen.setStatus("400 Bad Request");
	HGen.setType("text/html");
	HGen.setContentString(fileSTR);
	HGen.processResponse();
}

void Server::send_404(id_server_type serverNb){
	HGen.clear();
	std::string fileSTR = fileToString(serverNb, findPathError(serverNb, 404), true);//!raw until parsing done (path of error 404)

	HGen.setStatus("404 Not Found");
	HGen.setType("text/html");
	HGen.setContentString(fileSTR);
	HGen.processResponse();
}
