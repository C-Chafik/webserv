#include "includes.hpp"

std::string Server::treat_GET_request(struct header & header, id_server_type server_id, int clientFd){
	std::string rtnFile;
	std::string file = header.path;
	header.clientFd=clientFd;
	
	/*have to be the first check because can change the server_id*/
	check_server_name(header, server_id);

	file = parse_uri(header, server_id);

	if (file == "/")
		file = confs[server_id].index;

	rtnFile = fileLocation(file, server_id);//routing

	size_t ext = rtnFile.rfind(confs[server_id].cgi_extension);
	if ( ext != std::string::npos && (rtnFile[ext + confs[server_id].cgi_extension.size()] == '/' || !rtnFile[ext + confs[server_id].cgi_extension.size()]) ){
		php_cgi(header, server_id , rtnFile, "GET");
		return "";
	}

	redirect(rtnFile, server_id);
	
	return rtnFile;
}
