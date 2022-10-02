#include "includes.hpp"

void Server::php_cgi(struct header & header, id_server_type server_id, std::string script_name){
	std::string line;

	line.append("CONTENT_TYPE=");
	line.append(header.content_type);

	line.append(" QUERY_STRING=");
	line.append(confs[server_id].query_string);

	line.append(" SERVER_PROTOCOL=");
	line.append("HTTP/1.1");

	// line.append(" SERVER_SOFTWARE=");
	// line.append("WebServ");

	line.append(" SCRIPT_NAME=");
	line.append(script_name);
	

	// line.append(" SERVER_NAME=");
	// line.append(header.host);//!put the redirected server_name

	// line.append(" SERVER_PORT=");
	// line.append(header.port_host);//!put the redirected port

	line.append(" CONTENT_LENGTH=");
	if (header.content_length > 0)
		line.append(SSTR(header.content_length));

	
	line.append(" /usr/bin/php-cgi -f php-files/test.php");
	
	std::cout << "Command cgi : " << line << std::endl;//*log
	system(line.c_str());
}