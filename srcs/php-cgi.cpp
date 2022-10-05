#include "includes.hpp"

void Server::php_cgi(struct header & header, id_server_type server_id, std::string php_path, std::string method){
	std::string line;
	std::stringstream buffer;
	std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());

	line.append("CONTENT_TYPE=");
	line.append(header.content_type);

	line.append(" REDIRECT_STATUS=");
	line.append("");

	line.append(" REQUEST_METHOD=");
	line.append(method);

	line.append(" GATEWAY_INTERFACE=");
	line.append("CGI/1.1");

	line.append(" REDIRECT_STATUS=");
	line.append("200");

	line.append(" QUERY_STRING=");
	line.append(confs[server_id].query_string);

	line.append(" SERVER_PROTOCOL=");
	line.append("HTTP/1.1");

	line.append(" SERVER_SOFTWARE=");
	line.append("WebServ");

	line.append(" SCRIPT_NAME=");//parse php_path
	line.append(php_path.substr(0, php_path.find(confs[server_id].cgi_extension) + confs[server_id].cgi_extension.size()));

	std::string path_info;
	path_info = php_path.substr(php_path.find(confs[server_id].cgi_extension) + confs[server_id].cgi_extension.size(), php_path.size());
	line.append(" PATH_INFO=");
	line.append(path_info);
	

	line.append(" SERVER_NAME=");
	if (confs[server_id].server_names.size() > 0)
		line.append(confs[server_id].server_names[0]);
	else
		line.append(header.host);

	line.append(" SERVER_PORT=");
	line.append(confs[server_id].listening.begin()->second[0]);

	line.append(" PATH_TRANSLATED=");
	line.append(path_info);

	line.append(" CONTENT_LENGTH=");
	if (header.content_length > 0)
		line.append(SSTR(header.content_length));

	
	line.append(" php-cgi -q php-files/test.php > /tmp/output_webserv.tmp");
	

	system(line.c_str());
	std::cout << std::ifstream("/tmp/output_webserv.tmp").rdbuf();
	remove("/tmp/output_webserv.tmp");

	std::cout.rdbuf(old);
	std::cout << line << std::endl;//*log

	send_cgi(buffer.str());
}