#include "includes.hpp"

class Server{
	//struct
	struct serverInfo{
		int			serverSocket;
		sockaddr_in	serverSocketStruct;
	};

	struct parseLocation{
		std::string root;
	};

	struct parseGlobal{
		std::map<std::string/*location*/, struct parseLocation> location;
	};

	//var
	serverInfo serverInfo;
	fd_set current_connections;
	fd_set ready_connections;
	HeaderGen HGen;
	struct parseGlobal parseG;


	//func
	int accept_connection(struct serverInfo serverInfo);
	int treat_request( int requestFd );
	void handle_connection(int clientSocket, struct serverInfo serverInfo);
	std::string fileLocation(std::string request);
	std::string fileToString(std::string fileName, struct serverInfo serverInfo);
	void listenSocketServer(int port);

public:
	void run(int port);

};