#include "includes.hpp"

/**
 * take request file by client
 * return path of file to return
 *
 * return another path if redirection
 * return error file path if needed
 * 
 */
std::string Server::treat_GET_request(std::string file, id_server_type serverNb){
	std::string rtnFile;
	
	check_host();
	/**
	 *	- checkhostname
	 *	- routing
	 *	- check de la methode HTTP dans le path
	 *	- redirection
	 *	- check de repertoire
	 */
	rtnFile = fileLocation(file, serverNb);//routing
	
	return rtnFile;
}
