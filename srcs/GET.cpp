#include "includes.hpp"

std::string Server::autoindex( std::string URI )
{
	std::cout << "ENTERED AUTOINDEX " << std::endl;
	std::string listing;

	if (URI.empty() || *(URI.end() - 1) != '/')
		URI += "/";
	
	listing += "<html>\n<head><title>Index of /</title></head>\n<body bgcolor=\"white\">";
	listing += "\n<h1>Index of " + URI + "</h1><hr><pre>\n";

	//! Now retrieving every filename of the current folder

	std::cout << URI << std::endl;
	struct dirent *d;
	DIR *dir = opendir(URI.c_str());
	if ( dir )
		while ( (d = readdir(dir)) != NULL  )
			listing += "<a href=\"" + URI + d->d_name + "\">" + d->d_name + "</a>\n";
	closedir(dir);

	listing += "</pre><hr></body>\n";
	listing += "</html>";
	return listing;
}	

std::string Server::treat_GET_request(struct header & header, struct body & body, id_server_type server_id, int clientFd){
	std::string rtnFile;
	std::string location_name;
	std::string file = header.path;
	bool slash = false;
	bool autoindexed = false;
	header.clientFd=clientFd;

	location_name = targetLocation(header.path);
	if ( confs[server_id].locations.find(location_name) != confs[server_id].locations.end() )
		autoindexed = confs[server_id].locations[location_name].autoindex;

	if (*(header.path.end() - 1) == '/' )
		slash = true;

	/*have to be the first check because can change the server_id*/
	check_server_name(header, server_id);

	file = parse_uri(header, server_id);

	rtnFile = fileLocation(file, server_id);//routing

	if ( rtnFile.empty() )
		return confs[server_id].index;

	if ( slash == true && autoindexed == true ) //! IF autoindexed
	{
        rtnFile = autoindex(rtnFile);
		send_200_autoindex(rtnFile);
		return "";
	}

	size_t ext = rtnFile.rfind(confs[server_id].cgi_extension);
	if ( ext != std::string::npos && (rtnFile[ext + confs[server_id].cgi_extension.size()] == '/' || !rtnFile[ext + confs[server_id].cgi_extension.size()]) ){
		php_cgi(header, server_id , rtnFile, "GET", body);
		return "";
	}

	redirect(rtnFile, server_id);
	
	std::cout << MAGENTA << "[" << rtnFile << "]" << WHITE << std::endl;
	return rtnFile;
}
