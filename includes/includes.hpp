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
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <netdb.h>
#include <sys/socket.h>


struct config {
    std::vector<std::string> server_names; //! DONE
    std::map<std::string/*ip*/, std::vector<std::string> /*ports list*/> listening; //! DONE
    std::vector< std::pair<std::vector<int>, std::string> > errors; //! DONE
    int         body_max_size; //! DONE

    void clear( void )
    {
        server_names.clear();
        listening.clear();
        errors.clear();
        body_max_size = 0;
    }
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
