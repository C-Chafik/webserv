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
			std::cout << "Fail when opening : \"" << fileName << "\"" << std::endl;
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
