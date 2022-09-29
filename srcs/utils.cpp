#include "includes.hpp"

int Server::findServerIndex(int fdServer){
	for (std::string::size_type i = 0; i < server_sockets.size(); i++)
		if (server_sockets[i] == fdServer)
			return i;
	return -1;
}

void Server::exitCloseSock(){
	for (std::string::size_type i = 0; i < server_sockets.size() && server_sockets[i] != -1; i++)
		close(server_sockets[i]);
}

bool Server::wantToBeAccepted(int fd){
	for (std::string::size_type i = 0; i < server_sockets.size() && server_sockets[i] != -1; i++)
		if (server_sockets[i] == fd)
			return true;
	return false;
}

std::string Server::fileToString(std::string fileName, bool error){
	std::ifstream file;
	std::string	buffer;
	std::string	fileSTR;

	file.open(fileName.c_str());
	if (!file.is_open())
	{
		if (!file.is_open() && error){
			std::cout << "Fail when opening file" << std::endl;
			exitCloseSock();
			exit (EXIT_FAILURE);
		}
		else
			throw Error_page("404");
	}
	while (getline(file, buffer, '\n'))
	{
		fileSTR += buffer;
		fileSTR += "\n";
	}
	file.close();

	return fileSTR;
}

std::string Server::findPathError(id_server_type id_server, int error_code){
	std::vector< std::pair<std::vector<int>, std::string> >::iterator big_it = confs[id_server].errors.begin();
	for (; big_it != confs[id_server].errors.end(); big_it++){
		std::vector<int>::iterator lil_it = big_it->first.begin();
		for (; lil_it != big_it->first.end(); lil_it++)
			if (*lil_it == error_code)
				return big_it->second;
	}
	return "";
}
