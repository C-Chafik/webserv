#include "../includes/includes.hpp"

void	Server::treat_DELETE_request( struct header & head )
{
    std::string file_path = head.path;
    if ( file_path.size() > 2 && file_path[0] == '/')
		file_path.erase(0, 1); 
    std::cout << "TRYING TO DELETE THIS PATH -> " << file_path << std::endl;
    if ( file_already_exist(file_path) == true )
        remove(file_path.c_str());
}