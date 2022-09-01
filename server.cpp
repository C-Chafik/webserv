#include <sys/socket.h>//socket
#include <stdio.h>//fprintf
#include <stdlib.h>//exit
#include <netinet/in.h>//sockaddress_in
#include <iostream>//std::cerr
#include <unistd.h>//close
#include <signal.h>//signal
#include <fstream>//fstream

#include "HeaderGen.hpp"

int serverFd;
void endWell(int num){
	close(serverFd);
	exit(EXIT_SUCCESS);
}

sockaddr_in startListening(){
	serverFd = socket(AF_INET, SOCK_STREAM, 0);

	if (serverFd == -1){
		std::cerr << "Error when creating socket!" << std::endl;
		exit(EXIT_FAILURE);
	}

	signal(SIGINT, endWell);

	sockaddr_in serverInfo;
	serverInfo.sin_family = AF_INET;
	serverInfo.sin_addr.s_addr = INADDR_ANY;
	serverInfo.sin_port = htons(9998);

	if (bind(serverFd, reinterpret_cast<struct sockaddr *>(&serverInfo), sizeof(serverInfo)) < 0){
		std::cerr << "Error when binding socket to address!" << std::endl;
		exit(EXIT_FAILURE);
	}

	if (listen(serverFd, 10) == -1){
		std::cerr << "Error when listening for new connections!" << std::endl;
		exit(EXIT_FAILURE);
	}

	return serverInfo;
}

std::string fileToString(std::string fileName){
		std::ifstream file;
		std::string	buffer;
		std::string	fileSTR;

		file.open(fileName);
		if (!file.is_open())
		{
			std::cout << "Fail when creating file" << std::endl;
			close(serverFd);
			exit (EXIT_FAILURE);
		}
		while (getline(file, buffer, '\n'))
		{
			fileSTR += buffer;
			fileSTR += "\n";
		}

		return fileSTR;
}

void printRequest(int requestFd){
	char buff[1000];
	recv(requestFd, buff, 1000, 0);
	std::cout << buff << std::endl;
}

int main(){

	int clientFd;

	sockaddr_in serverInfo = startListening();

	socklen_t serverInfoSize = static_cast<socklen_t>(sizeof(serverInfo));


	while (1){
		HeaderGen HGen;

		clientFd = accept(serverFd, reinterpret_cast<struct sockaddr *>(&serverInfo), &serverInfoSize );

		if (clientFd == -1){
			std::cerr << "Error when accepting new connetion!" << std::endl;
		}

		printRequest(clientFd);

		std::string fileSTR = fileToString("test.html");

		HGen.setStatus("200 OK");
		HGen.setType("text/html");
		HGen.setContentString(fileSTR);
		HGen.processResponse();

		std::string response = HGen.getStr();
		send(clientFd, response.c_str(), response.size(), SOCK_DGRAM);

		close(clientFd);
	}
	close(serverFd);
	
	return 0;
}
