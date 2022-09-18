#include "includes.hpp"

int Server::accept_connection(int fdServer){
	int clientFd;
	int serverIndex = findServerIndex(fdServer);
	socklen_t serverInfoSize = static_cast<socklen_t>(sizeof(server_sockets[serverIndex]));

	std::cout << "param  : " << fdServer << std::endl;
	std::cout << "server : " << server_sockets[serverIndex] << std::endl;
	clientFd = accept(server_sockets[serverIndex], reinterpret_cast<struct sockaddr *>(&server_sockets_struct[serverIndex])
		, &serverInfoSize);
	if (clientFd == -1){
		std::cerr << "Error when accept new connection!" << std::endl;
		exitCloseSock();
		exit (EXIT_FAILURE);
	}

	return clientFd;
}
