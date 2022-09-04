#ifndef INCLUDES_HPP
#define INCLUDES_HPP

#include <string>
#include "request_handler.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <cstring>
#include <utility>

#include "HeaderGen.hpp"
#include "request_handler.hpp"

enum    METHOD{
    GET,
    POST,
    DELETE
};

struct serverInfo{
    int			serverSocket;
    sockaddr_in	serverSocketStruct;
};

struct config{
    std::string server_name;
    std::string location_get_dir;
    int         listen_address;
    int         listen_port;
    int         body_max_size;
    std::vector< std::pair<std::vector<int>, std::string> > errors;
}

int accept_connection(struct serverInfo serverInfo);
int treat_request( int requestFd );
void handle_connection(int clientSocket, struct serverInfo serverInfo);
std::string fileToString(std::string fileName, struct serverInfo serverInfo);


#endif
