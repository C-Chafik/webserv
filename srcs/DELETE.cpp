#include "../includes/includes.hpp"

void	Server::treat_DELETE_request( struct header & head )
{
    std::cout << "TRYING TO DELETE THIS PATH -> " << head.path << std::endl;
    if ( file_already_exist(head.path) == true )
        remove(head.path.c_str());
}