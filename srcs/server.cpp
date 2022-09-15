#define PORT 9999
#define BACKLOG 10

#include "includes.hpp"

std::string Server::fileToString(std::string fileName, struct serverInfo serverInfo, bool error){
	std::ifstream file;
	std::string	buffer;
	std::string	fileSTR;

	file.open(fileName);
	if (!file.is_open())
	{
		if (!file.is_open() && error){
			std::cout << "Fail when opening file" << std::endl;
			close(serverInfo.serverSocket);
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

void Server::listenSocketServer(int port){
	/*listen multiple ports : https://stackoverflow.com/questions/15560336/listen-to-multiple-ports-from-one-server*/
	serverInfo.serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (serverInfo.serverSocket == -1){
		std::cerr << "Error when creating server's socket!" << std::endl;
		exit (EXIT_FAILURE);
	}

	int value = 1;
	if (setsockopt(serverInfo.serverSocket, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value)) < 0)
		exit (EXIT_FAILURE);

	serverInfo.serverSocketStruct.sin_family = AF_INET;
	serverInfo.serverSocketStruct.sin_addr.s_addr = INADDR_ANY;
	serverInfo.serverSocketStruct.sin_port = htons(port);

	if (bind(serverInfo.serverSocket,
		reinterpret_cast<struct sockaddr *>(&serverInfo.serverSocketStruct),
		sizeof(serverInfo.serverSocketStruct)) == -1){
			std::cerr << "Port " << conf.listening.begin()->second[0] << " : " << "Error when binding socket and address!" << std::endl;
			close (serverInfo.serverSocket);
			exit (EXIT_FAILURE);
	}

	if (listen(serverInfo.serverSocket, BACKLOG) == -1){
			std::cerr << "Error when server started listening!" << std::endl;
			close (serverInfo.serverSocket);
			exit (EXIT_FAILURE);
	}
}

void Server::run(){

	std::map<std::string, std::vector<std::string> >::iterator listen_it = conf.listening.begin();


	listenSocketServer(atoi(listen_it->second[0].c_str()));

	FD_ZERO(&current_connections);//init struct to 0
	FD_SET(serverInfo.serverSocket, &current_connections);//set the server socket fd in current connection struct

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
			close (serverInfo.serverSocket);
			exit (EXIT_FAILURE);
		}

		for (int i = 0; i < FD_SETSIZE; i++){//check all fd possible in ready_connection struct
			if (FD_ISSET(i, &ready_connections)){
				if (i == serverInfo.serverSocket){//new connection wait to be taken at the server.socket ip
					int clientSocket = accept_connection(serverInfo);
					FD_SET(clientSocket,  &current_connections);//set new connection established in the current_connection struct
				}
				else{
					handle_connection(i, serverInfo);
					FD_CLR(i, &current_connections);//remove the connection after everything done
				}
			}
		}
	}
}
