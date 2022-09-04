#define PORT 9999
#define BACKLOG 10

#include "includes.hpp"

std::string fileToString(std::string fileName, struct serverInfo serverInfo){
	std::ifstream file;
	std::string	buffer;
	std::string	fileSTR;

	file.open(fileName);
	if (!file.is_open())
	{
		std::cout << "Fail when opening file" << std::endl;
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

<<<<<<< HEAD:srcs/server.cpp
=======
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

int treat_request( int requestFd )
{
    char header[4096 + 1];
	size_t end;

	memset(header, 0, 4096);
	while ( (end = recv(requestFd, header, 4096 - 1, 0)) > 0 )
	{
		if ( header[end - 1] == '\n' )
			break ;
		memset(header, 0, 4096);
	}
	if ( end == -1 )
	{
		std::cout << "ERROR RECEIVING THE HEADER " << std::endl;
		exit(0);
	}
	std::cout << YELLOW << header << WHITE << std::endl;

    request_handler request(header);
    
    request.parse_header();

    return request.state;
}

void handle_connection(int clientSocket, struct serverInfo serverInfo){

	if ((treat_request(clientSocket) == 0))
	{
		std::cout << "ERROR IN TREATING REQUEST" << std::endl;
		return ;
	}
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

>>>>>>> 4db075aed18e4ae6c34402c8384ba735fd46d785:server.cpp
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
