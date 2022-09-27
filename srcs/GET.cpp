#include "includes.hpp"

/**
 * take request file by client
 * return path of file to return
 *
 * return another path if redirection
 * return error file path if needed
 * 
 */
std::string Server::treat_GET_request(struct request &req, id_server_type serverNb){
	std::string rtnFile;
	
	/*have to be the first check because can change the serverNb*/
	check_server_name(req, serverNb);

	std::string file = parse_uri(req);

	if (file == "/")
		file = "index.html";

	rtnFile = fileLocation(file, serverNb);//routing
	
	return rtnFile;
}
