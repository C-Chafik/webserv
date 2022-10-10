#include "../includes/includes.hpp"

void	Server::treat_DELETE_request( struct header & head, id_server_type server_id )
{
    std::string file_path = head.path;
    if ( file_path.size() >= 2 && file_path[0] == '/')
		  file_path.erase(0, 1); 

    if ( file_already_exist(file_path) == true )
        if ( unlink(file_path.c_str()) == -1 )
            send_500(server_id);
}