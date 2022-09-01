#include <sys/socket.h>//socket
#include <stdio.h>//fprintf
#include <stdlib.h>//exit
#include <netinet/in.h>//sockaddress_in
#include <iostream>//std::cerr
#include <unistd.h>//close
#include <signal.h>




int serverFd;
void endWell(int num){
	close(serverFd);
	exit(EXIT_SUCCESS);
}

int main(){

	int clientFd;

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

	socklen_t serverInfoSize = static_cast<socklen_t>(sizeof(serverInfo));

	while (1){

		clientFd = accept(serverFd, reinterpret_cast<struct sockaddr *>(&serverInfo), &serverInfoSize );

		if (clientFd == -1){
			std::cerr << "Error when accepting new connetion!" << std::endl;
		}

		char buff[1000];
		recv(clientFd, buff, 1000, 0);
		std::cout << buff << std::endl;

		std::string response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 88\n\n<html>\n<head><title>test</title></head>\n<body>\n<h1>Title test</h1>\n</body>\n<html>\n";
		send(clientFd, response.c_str(), response.size(), SOCK_DGRAM);

		close(clientFd);

	}
	close(serverFd);
	
	return 0;
}