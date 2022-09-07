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
#include <thread>
#include <signal.h>
#include <map>

struct config{
    std::vector<std::string> server_names;
    std::string location_get_dir;
    // std::string         listen_address;
    // std::string         listen_port;
    std::map<std::string, std::string> listening;
    int         body_max_size;
    std::vector< std::pair<std::vector<int>, std::string> > errors;
};

#include "parseConfig.hpp"
#include "HeaderGen.hpp"
#include "request_handler.hpp"
#include "server.hpp"

enum    METHOD{
    GET,
    POST,
    DELETE
};



#endif
