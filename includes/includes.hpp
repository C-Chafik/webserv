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

#include "parseConfig.hpp"
#include "HeaderGen.hpp"
#include "request_handler.hpp"
#include "server.hpp"

enum    METHOD{
    GET,
    POST,
    DELETE
};

struct config{
    std::string server_name;
    std::string location_get_dir;
    std::string         listen_address;
    std::string         listen_port;
    int         body_max_size;
    std::vector< std::pair<std::vector<int>, std::string> > errors;
};


#endif
