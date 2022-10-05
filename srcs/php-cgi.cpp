#include "includes.hpp"

void Server::php_cgi(struct header & header, id_server_type server_id, std::string script_name){
	std::string line;
	std::stringstream buffer;
	std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());

	// line.append("CONTENT_TYPE=");
	// line.append(header.content_type);

	// line.append(" REDIRECT_STATUS=");
	// line.append("200");

	line.append(" QUERY_STRING=");
	line.append(confs[server_id].query_string);

	// line.append(" SERVER_PROTOCOL=");
	// line.append("HTTP/1.1");

	// line.append(" SERVER_SOFTWARE=");
	// line.append("WebServ");//*redirect

	// line.append(" SCRIPT_NAME=");
	// line.append(script_name);
	(void)script_name;

	line.append(" PATH_INFO=");
	line.append(confs[server_id].cgi_path);
	

	line.append(" SERVER_NAME=");
	if (confs[server_id].server_names.size() > 0)
		line.append(confs[server_id].server_names[0]);
	else
		line.append(header.host);

	line.append(" SERVER_PORT=");
	line.append(confs[server_id].listening.begin()->second[0]);

	line.append(" CONTENT_LENGTH=");
	if (header.content_length > 0)
		line.append(SSTR(header.content_length));

	
	line.append(" php-cgi -q php-files/test.php > /tmp/output_webserv.tmp");
	

	system(line.c_str());
	std::cout << std::ifstream("/tmp/output_webserv.tmp").rdbuf();
	remove("/tmp/output_webserv.tmp");

	std::cout.rdbuf(old);

	send_cgi(buffer.str());
	
}