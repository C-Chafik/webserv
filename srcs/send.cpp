#include "includes.hpp"

/**
 * create response with a 200 Header
 * Do the routing of the path
 * return 404 if not found
 */
void Server::send_200(std::string file, id_server_type server_id){
	HGen.clear();
	std::string fileSTR = fileToString(server_id, file);

	HGen.setStatus("200 OK");
	HGen.setType("text/html");
	HGen.setContentString(fileSTR);
	HGen.processResponse();
}

void Server::send_400(id_server_type server_id){
	HGen.clear();
	std::string fileSTR = fileToString(server_id, findPathError(server_id, 400), true);//!raw until parsing done (path of error 400)

	HGen.setStatus("400 Bad Request");
	HGen.setType("text/html");
	HGen.setContentString(fileSTR);
	HGen.processResponse();
}

void Server::send_404(id_server_type server_id){
	HGen.clear();
	std::string fileSTR = fileToString(server_id, findPathError(server_id, 404), true);//!raw until parsing done (path of error 404)

	HGen.setStatus("404 Not Found");
	HGen.setType("text/html");
	HGen.setContentString(fileSTR);
	HGen.processResponse();
}

void Server::send_301(id_server_type server_id, std::string path){
	HGen.clear();

	HGen.setStatus("301 Moved Permanently");
	HGen.setLocation(confs[server_id].locations[path].http_redirection.second);
	HGen.processResponse();
}

void Server::send_302(){
	HGen.clear();

	HGen.setStatus("302 Found");
	HGen.processResponse();
}
