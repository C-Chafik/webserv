#include "includes.hpp"

void Server::send_200(std::string file){
	std::string fileSTR = fileToString(file, serverInfo);

	HGen.setStatus("200 OK");
	HGen.setType("text/html");
	HGen.setContentString(fileSTR);
	HGen.processResponse();
}

void Server::send_400(){
	std::string fileSTR = fileToString(parseG.path_e_400, serverInfo);//!raw until parsing done (path of error 400)

	HGen.setStatus("400 Bad Request");
	HGen.setType("text/html");
	HGen.setContentString(fileSTR);
	HGen.processResponse();
}

void Server::send_404(){
	std::string fileSTR = fileToString(parseG.path_e_404, serverInfo);//!raw until parsing done (path of error 404)

	HGen.setStatus("400 Bad Request");
	HGen.setType("text/html");
	HGen.setContentString(fileSTR);
	HGen.processResponse();
}

void Server::send_index(){
	std::ifstream file;
	std::string	buffer;
	std::string	fileSTR;

	for (int i = 0; i < parseG.index.size(); i++){
		file.open(parseG.index[i]);
		if (file.is_open())
			break;
	}

	if (!file.is_open())
		send_404();
	
	while (getline(file, buffer, '\n'))
	{
		fileSTR += buffer;
		fileSTR += "\n";
	}
	file.close();
}