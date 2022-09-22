#include "includes.hpp"

void Server::handle_connection(int clientSocket, id_server_type server_id){
	std::cout << "server id : " << server_id << std::endl;

	int method;
	method = treat_request(clientSocket);
	
	if (check_host() == D_400)
		send_400(server_id);
	else {

		if ( method == GET ){
			std::cout << CYAN << "METHOD = GET " << WHITE << std::endl;
			/*treat all the config stuuf in a function here*/
			send_200("index.html", server_id);//! do the file dynamic
		}
		else if ( method == POST )
		{
			std::cout << CYAN << "METHOD POST RECEIVED ! " << WHITE << std::endl;
			treat_POST_request(_header);
		}
		else if ( method == DELETE )
			std::cout << CYAN << "METHOD = DELETE " << WHITE << std::endl;
		else
		{
			std::cout << CYAN << "SOMETHING WENT WRONG WITH THE REQUEST " << WHITE << std::endl;
			// exit(0);
		}
	}


	std::string response = HGen.getStr();
	send(clientSocket, response.c_str(), response.size(), SOCK_DGRAM);
}
