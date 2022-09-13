#ifndef SERVER_HPP
#define SERVER_HPP

#include "includes.hpp"

#define D_200 0
#define D_400 1

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
		in_addr_t ip_address;
		std::vector<std::string> server_names;
		std::string path_e_404;//init default path or parsed value
		std::string path_e_400;//init default path or parsed value
			std::vector<std::string> index;
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
	std::string fileToString(std::string fileName, struct serverInfo serverInfo, bool error = false);
	void listenSocketServer(int port);
	void check_host();
	short host(in_addr_t ip_host, std::string name_host);// return if send 200 300 400 ...
	void send_200(std::string file);
	void send_index();
	void send_400();
	void send_404();

public:
	struct config conf;
	void run();

};

#endif