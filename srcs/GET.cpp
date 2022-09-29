#include "includes.hpp"

/**
 * take request file by client
 * return path of file to return
 *
 * return another path if redirection
 * return error file path if needed
 * 
 */
std::string Server::treat_GET_request(struct request &req, id_server_type server_id){
	std::string rtnFile;
	
	/*have to be the first check because can change the server_id*/
	check_server_name(req, server_id);

	std::string file = parse_uri(req, server_id);

	if (file == "/")
		file = "index.html";

	rtnFile = fileLocation(file, server_id);//routing
	
	return rtnFile;
}
