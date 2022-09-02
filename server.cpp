#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <fstream>

#include "HeaderGen.hpp"
#include "request_handler.hpp"

#define PORT 9999
#define BACKLOG 10


struct serverInfo{
	int			serverSocket;
	sockaddr_in	serverSocketStruct;
};

std::string fileToString(std::string fileName, struct serverInfo serverInfo){
	std::ifstream file;
	std::string	buffer;
	std::string	fileSTR;

	file.open(fileName);
	if (!file.is_open())
	{
		std::cout << "Fail when creating file" << std::endl;
		close(serverInfo.serverSocket);
		exit (EXIT_FAILURE);
	}
	while (getline(file, buffer, '\n'))
	{
		fileSTR += buffer;
		fileSTR += "\n";
	}

	return fileSTR;
}

struct serverInfo listenSocketServer(){
	struct serverInfo serverInfo;

	serverInfo.serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (serverInfo.serverSocket == -1){
		std::cerr << "Error when creating server's socket!" << std::endl;
		exit (EXIT_FAILURE);
	}

	serverInfo.serverSocketStruct.sin_family = AF_INET;
	serverInfo.serverSocketStruct.sin_addr.s_addr = INADDR_ANY;
	serverInfo.serverSocketStruct.sin_port = htons(PORT);

	if (bind(serverInfo.serverSocket,
		reinterpret_cast<struct sockaddr *>(&serverInfo.serverSocketStruct),
		sizeof(serverInfo.serverSocketStruct)) == -1){
			std::cerr << "Error when binding socket and address!" << std::endl;
			close (serverInfo.serverSocket);
			exit (EXIT_FAILURE);
	}

	if (listen(serverInfo.serverSocket, BACKLOG) == -1){
			std::cerr << "Error when server started listening!" << std::endl;
			close (serverInfo.serverSocket);
			exit (EXIT_FAILURE);
	}

	return serverInfo;
}

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

void handle_connection(int clientSocket, struct serverInfo serverInfo){
	char buff[1000];
	recv(clientSocket, buff, 1000, 0);
	std::cout << buff << std::endl;

	//traitement de la request

	HeaderGen HGen;

	std::string fileSTR = fileToString("test.html", serverInfo);

	HGen.setStatus("200 OK");
	HGen.setType("text/html");
	HGen.setContentString(fileSTR);
	HGen.processResponse();

	std::string response = HGen.getStr();
	send(clientSocket, response.c_str(), response.size(), SOCK_DGRAM);
}

int main(){
	struct serverInfo serverInfo;

	serverInfo = listenSocketServer();

	fd_set current_connections;
	fd_set ready_connections;

	FD_ZERO(&current_connections);
	FD_SET(serverInfo.serverSocket, &current_connections);

	while (true){
		ready_connections = current_connections;

		if (select(FD_SETSIZE, &ready_connections, NULL, NULL, NULL) == -1){
			std::cerr << "Error when select ready sockets!" << std::endl;
			close (serverInfo.serverSocket);
			exit (EXIT_FAILURE);
		}

		for (int i = 0; i < FD_SETSIZE; i++){
			if (FD_ISSET(i, &ready_connections)){
				if (i == serverInfo.serverSocket){
					int clientSocket = accept_connection(serverInfo);
					FD_SET(clientSocket,  &current_connections);
				}
				else{
					handle_connection(i, serverInfo);
					FD_CLR(i, &current_connections);
				}
			}
		}
	}

	return (EXIT_SUCCESS);
}
