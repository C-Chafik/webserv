#include "includes.hpp"

/**
 * take request file by client
 * return path of file to return
 *
 * return another path if redirection
 * return error file path if needed
 * 
 */
std::string Server::treat_GET_request(struct header & header, id_server_type server_id){
	std::string rtnFile;
	std::string file = header.path;
	
	/*have to be the first check because can change the server_id*/
	check_server_name(header, server_id);


	file = parse_uri(header, server_id);

	if (file == "/")
		file = "index.html";

	rtnFile = fileLocation(file, server_id);//routing

	php_cgi(server_id);

	redirect(rtnFile, server_id);
	
	return rtnFile;
}
