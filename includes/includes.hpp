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
#include <signal.h>
#include <map>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <netdb.h>
#include <sys/socket.h>
#include <sstream>
#include "common.hpp"

#define SSTR( x ) static_cast< std::ostringstream & >(( std::ostringstream() << std::dec << x ) ).str()



struct parseLocation {
        bool GET;
        bool POST;
        bool DELETE;
        std::string root;
        std::string upload_path;
        bool autoindex;
        std::pair< int, std::string > http_redirection;

        parseLocation( void ) { autoindex = false; GET = false; POST = false; DELETE = false; }
        ~parseLocation ( void ) {}
};

struct parsePOST {
		size_t content_length;
		std::string content_type;
		std::string content;

		parsePOST( void ) { content_length = 0; }
        ~parsePOST( void ) {}
		void clear( void )
		{
			content_length = 0;
			content_type.clear();
			content.clear();
		}
};


struct config {
    std::vector<std::string> server_names;
    std::map<std::string/*ip*/, std::vector<std::string> /*ports list*/> listening;
    std::vector< std::pair<std::vector<int>, std::string> > errors;
    std::map< std::string, struct parseLocation > locations;
    int         body_max_size;

    config( void ) { body_max_size = 100000; }
    ~config ( void ) {}

    void clear( void )
    {
        locations.clear();
        server_names.clear();
        listening.clear();
        errors.clear();
        body_max_size = 100000;
    }
};

struct request{
    std::string host;
    std::string port_host;
};


#include "parseConfig.hpp"
#include "HeaderGen.hpp"
#include "request_handler.hpp"
#include "server.hpp"

enum    METHOD{
    GET,
    POST,
    DELETE,
    UNKNOWN
};

#endif
