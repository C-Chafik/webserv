#include "includes.hpp"

void Server::send_200_autoindex(std::string content){
	HGen.clear();
	HGen.setStatus("200 OK");
	HGen.setType("text/html");
	HGen.setContentString(content);
	HGen.processResponse();
}

void Server::send_200(std::string file){
	HGen.clear();
	std::string fileSTR = fileToString(file);

	HGen.setStatus("200 OK");
	HGen.setType("text/html");
	HGen.setContentString(fileSTR);
	HGen.processResponse();
}

void Server::send_202(void){
	HGen.clear();

	HGen.setStatus("202 Accepted");
	HGen.processResponse();
}

void Server::send_400(id_server_type serverNb){
	HGen.clear();
	std::string fileSTR = fileToString(confs[serverNb].errors[400], true);

	HGen.setStatus("400 Bad Request");
	HGen.setType("text/html");
	HGen.setContentString(fileSTR);
	HGen.processResponse();
}

void Server::send_413(id_server_type serverNb){
	HGen.clear();
	std::string fileSTR = fileToString(confs[serverNb].errors[413], true);

	HGen.setStatus("413 Bad Request");
	HGen.setType("text/html");
	HGen.setContentString(fileSTR);
	HGen.processResponse();
}

void Server::send_404(id_server_type serverNb){
	HGen.clear();
	std::string fileSTR = fileToString(confs[serverNb].errors[404], true);

	HGen.setStatus("404 Not Found");
	HGen.setType("text/html");
	HGen.setContentString(fileSTR);
	HGen.processResponse();
}

void Server::send_500(id_server_type serverNb){
	HGen.clear();
	std::string fileSTR = fileToString(confs[serverNb].errors[500], true);

	HGen.setStatus("500 Internal Server Error");
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

void Server::send_cgi(id_server_type server_id, std::string data){
	if (cgi_error(server_id)){
		return;
	}
	
	if (HGen.checkStatus())
		HGen.setStatus("200 OK");
	HGen.setContentString(data);
	
	HGen.processResponse();
}