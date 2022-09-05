#include "includes.hpp"

class Server{
	//struct
	struct serverInfo{
		int			serverSocket;
		sockaddr_in	serverSocketStruct;
	};

	//var
	serverInfo serverInfo;
	fd_set current_connections;
	fd_set ready_connections;
	HeaderGen HGen;


	//func
	int accept_connection(struct serverInfo serverInfo);
	int treat_request( int requestFd );
	void handle_connection(int clientSocket, struct serverInfo serverInfo);
	std::string fileToString(std::string fileName, struct serverInfo serverInfo);
	void listenSocketServer(int port);

public:
	void run(int port);

};