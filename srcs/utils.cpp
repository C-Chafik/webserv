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
