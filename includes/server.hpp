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

	typedef std::vector< struct config >::size_type id_server_type;

	//var
	std::string _header;
	std::vector< struct config > confs;
	std::vector<int> server_sockets;
	std::vector< std::vector< struct config >::size_type > socket_to_server;
	std::map<int/*client fd*/, int/*server_id*/> confs_index;
	std::vector<sockaddr_in> server_sockets_struct;
	fd_set current_connections;//fd waiting to communicate
	fd_set ready_connections;//fd ready to communicate
	HeaderGen HGen;
	struct parseGlobal parseG;


	//func
	int accept_connection(int fdServer);
	int treat_request( int requestFd );
	void handle_connection(int clientSocket, id_server_type server_id);
	std::string findPathError(id_server_type id_server, int errorCode);
	std::string fileLocation(std::string request, id_server_type serverNb);
	std::string fileToString(id_server_type server_id, std::string fileName, bool error = false);
	void listenSocketServer();
	short check_host();
	short host(in_addr_t ip_host, std::string name_host);// return if send 200 300 400 ...
	void send_200(std::string file, id_server_type serverNb);
	void send_400(id_server_type serverNb);
	void send_404(id_server_type serverNb);
	bool isIpAddress(std::string addr);
	bool hostToIp(std::string hostname);
	int findServerIndex(int fdServer);
	void exitCloseSock();
	bool wantToBeAccepted(int fd);



	//! POST

	
	//! It create the POST Object
	bool treat_POST_request( const std::string & header );



public:
	void run(std::vector< struct config > confs);

};

#endif