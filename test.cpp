#include <sys/socket.h>//socket
#include <stdio.h>//fprintf
#include <stdlib.h>//exit
#include <netinet/in.h>//sockaddress_in
#include <iostream>//std::cerr
#include <unistd.h>//close
#include <signal.h>


int connection;
int socketFd;

void endWell(int num){
	close(connection);
	close(socketFd);
}

int main(){

	socketFd = socket(AF_INET, SOCK_STREAM, 0);

	if (socketFd == -1){
		std::cerr << "Error when creating socket!" << std::endl;
		exit(EXIT_FAILURE);
	}

	signal(SIGINT, endWell);

	sockaddr_in socketInfo;
	socketInfo.sin_family = AF_INET;
	socketInfo.sin_addr.s_addr = INADDR_ANY;
	socketInfo.sin_port = htons(9999);

	if (bind(socketFd, reinterpret_cast<struct sockaddr *>(&socketInfo), sizeof(socketInfo)) < 0){
		std::cerr << "Error when binding socket to address!" << std::endl;
		exit(EXIT_FAILURE);
	}

	if (listen(socketFd, 10) == -1){
		std::cerr << "Error when listening for new connections!" << std::endl;
		exit(EXIT_FAILURE);
	}

	socklen_t socketInfoSize= static_cast<socklen_t>(sizeof(socketInfo));

	connection = accept(socketFd, reinterpret_cast<struct sockaddr *>(&socketInfo), &socketInfoSize);

	if (connection == -1){
		std::cerr << "Error when accepting new connetion!" << std::endl;
	}

	char buff[1000];
	recv(connection, buff, 1000, 0);
	std::cout << buff << std::endl;

	std::string response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!\n";
	send(connection, response.c_str(), response.size(), SOCK_DGRAM);

	
	return 0;
}