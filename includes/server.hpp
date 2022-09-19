#ifndef SERVER_HPP
#define SERVER_HPP

#include "includes.hpp"

#define D_200 0
#define D_400 1

class Server{
	//struct
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
	std::vector<int> server_sockets;
	std::vector<sockaddr_in> server_sockets_struct;
	fd_set current_connections;//fd waiting to communicate
	fd_set ready_connections;//fd ready to communicate
	HeaderGen HGen;
	struct parseGlobal parseG;


	//func
	int accept_connection(int fdServer);
	int treat_request( int requestFd );
	void handle_connection(int clientSocket);
	std::string fileLocation(std::string request);
	std::string fileToString(std::string fileName, bool error = false);
	void listenSocketServer();
	void check_host();
	short host(in_addr_t ip_host, std::string name_host);// return if send 200 300 400 ...
	void send_200(std::string file);
	void send_index();
	void send_400();
	void send_404();
	bool isIpAddress(std::string addr);
	bool hostToIp(std::string hostname);
	int findServerIndex(int fdServer);
	void exitCloseSock();
	bool wantToBeAccepted(int fd);



public:
	struct config conf;
	void run();

};

#endif