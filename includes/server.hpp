#ifndef SERVER_HPP
# define SERVER_HPP

# define D_200 0
# define D_400 1

# include "includes.hpp"

class Server{
	//exceptions
	class Error_page : public std::exception{
		public:
		explicit Error_page(const char* error_msg) : msg(error_msg) {}
		explicit Error_page(const std::string& error_msg) : msg(error_msg) {}
		virtual ~Error_page() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW {}

		virtual const char* what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW{
			return msg.c_str();
		}

		protected:
		std::string msg;
	};

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
	fd_set write_current_connections;
	fd_set write_ready_connections;//! en faire un tableau
	fd_set error_current_connections;
	fd_set error_ready_connections;
	HeaderGen HGen;
	std::map<id_server_type/*socket id*/, class Request /*Request Object*/> all_request;
	struct parseGlobal parseG;


	//func
	int accept_connection(int fdServer);
	void			receive_request_body( struct request & req, int requestFd );
	bool handle_connection(int clientSocket, id_server_type server_id);
	std::string findPathError(id_server_type id_server, int errorCode);
	std::string fileLocation(std::string request, id_server_type serverNb);
	std::string fileToString(id_server_type server_id, std::string fileName, bool error = false);
	void listenSocketServer();
	std::string ipToHost(std::string hostname);
	void check_server_name(struct header & header, id_server_type &id);
	void send_200(std::string file, id_server_type serverNb);
	void send_400(id_server_type serverNb);
	void send_404(id_server_type serverNb);
	bool isIpAddress(std::string addr);
	bool hostToIp(std::string hostname);
	int findServerIndex(int fdServer);
	void exitCloseSock();
	bool wantToBeAccepted(int fd);



	//* GET
	std::string treat_GET_request(struct header & header, id_server_type serverNb);


	//* POST
	//* It create the POST Object
	std::string get_file_name( const std::string & line );
	void treat_POST_request( struct header & head, struct body & bod );



public:
	void run(std::vector< struct config > confs);

};

#endif