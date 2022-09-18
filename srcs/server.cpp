#define PORT 9999
#define BACKLOG 10

#include "includes.hpp"

std::string Server::fileToString(std::string fileName, bool error){
	std::ifstream file;
	std::string	buffer;
	std::string	fileSTR;

	file.open(fileName);
	if (!file.is_open())
	{
		if (!file.is_open() && error){
			std::cout << "Fail when opening file" << std::endl;
			exitCloseSock();
			exit (EXIT_FAILURE);
		}
		else
			send_404();
	}
	while (getline(file, buffer, '\n'))
	{
		fileSTR += buffer;
		fileSTR += "\n";
	}
	file.close();

	return fileSTR;
}

void Server::listenSocketServer(){
	std::map<std::string,std::vector<std::string>>::iterator it = conf.listening.begin();

	while (it != conf.listening.end()){//for all addresses
		for (int i = 0; i < it->second.size(); i++){//for all port of address
			sockaddr_in	serverSocketStruct;

			server_sockets.push_back( socket(AF_INET, SOCK_STREAM, 0) );

			if (server_sockets.at(server_sockets.size() - 1) == -1){
				for (int j = 0; j < server_sockets.size() && server_sockets[j] != -1; j++)
					close(server_sockets[j]);
				std::cerr << "Error when creating server's socket!" << std::endl;
				exit (EXIT_FAILURE);
			}

			int value = 1;
			if (setsockopt(server_sockets.at(server_sockets.size() - 1), SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value)) < 0)
				exit (EXIT_FAILURE);

			serverSocketStruct.sin_family = AF_INET;
			serverSocketStruct.sin_addr.s_addr = INADDR_ANY;
			serverSocketStruct.sin_port = htons(atoi(it->second[i].c_str()));

			server_sockets_struct.push_back( serverSocketStruct );


			std::cout << it->second[i] << std::endl;

			if (bind(server_sockets.at(server_sockets.size() - 1),
				reinterpret_cast<struct sockaddr *>(&serverSocketStruct),
				sizeof(serverSocketStruct)) == -1){
					std::cerr << "Error when binding socket and address!" << std::endl;
					for (int j = 0; j < server_sockets.size() && server_sockets[j] != -1; j++)
						close(server_sockets[j]);
					exit (EXIT_FAILURE);
			}

			if (listen(server_sockets.at(server_sockets.size() - 1), BACKLOG) == -1){
					std::cerr << "Error when server started listening!" << std::endl;
					exitCloseSock();
					exit (EXIT_FAILURE);
			}
		}
		++it;
	}
}

void Server::run(){

	std::map<std::string, std::vector<std::string> >::iterator listen_it = conf.listening.begin();


	listenSocketServer();

	FD_ZERO(&current_connections);//init struct to 0
	for (int i = 0; i < server_sockets.size(); i++)
		FD_SET(server_sockets[i], &current_connections);//set the server socket fd in current connection struct


	/*raw data while waiting parsing*/
	parseG.location["/"].root = "html_files/";// tmp attend pour parsing autofill && add setter to add / back if needed
	parseG.location["error_files/"].root = "html_files/error_files/";// tmp attend pour parsing autofill && add setter to add / back if needed
	parseG.path_e_400 = fileLocation("error_files/error_400.html");
	parseG.path_e_404 = fileLocation("error_files/error_404.html");
	parseG.index.push_back(fileLocation("index.html"));
	/*END raw data while waiting parsing*/

	while (true){//server loop
		ready_connections = current_connections;

		/*get ready for connection fd in ready connection struct*/
		if (select(FD_SETSIZE, &ready_connections, NULL, NULL, NULL) == -1){
			std::cerr << "Error when select ready sockets!" << std::endl;
			exitCloseSock();
			exit (EXIT_FAILURE);
		}

		for (int i = 0; i < FD_SETSIZE; i++){//check all fd possible in ready_connection struct
			if (FD_ISSET(i, &ready_connections)){
				if (wantToBeAccepted(i)){//new connection wait to be taken at the server.socket ip
					std::cout << "test accept\n";
					int clientSocket = accept_connection(i);
					FD_SET(clientSocket,  &current_connections);//set new connection established in the current_connection struct
				}
				else{
					std::cout << "test handle\n";
					handle_connection(i);
					FD_CLR(i, &current_connections);//remove the connection after everything done
				}
			}
		}
	}
}
