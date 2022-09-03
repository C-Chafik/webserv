#include "includes.hpp"

int accept_connection(struct serverInfo serverInfo){
	int clientFd;
	socklen_t serverInfoSize = static_cast<socklen_t>(sizeof(serverInfo.serverSocketStruct));

	clientFd = accept(serverInfo.serverSocket, reinterpret_cast<struct sockaddr *>(&serverInfo.serverSocketStruct)
		, &serverInfoSize);
	if (clientFd == -1){
		std::cerr << "Error when accept new connection!" << std::endl;
		close (serverInfo.serverSocket);
		exit (EXIT_FAILURE);
	}

	return clientFd;
}
