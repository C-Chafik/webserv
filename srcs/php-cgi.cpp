#include "includes.hpp"

std::string Server::cgi_vars(struct header & header, id_server_type server_id, std::string php_path, std::string method){
	std::string line;

	line.append("CONTENT_TYPE=" + header.content_type);
	line.append(" REDIRECT_STATUS=");
	(void) method;
	// line.append(" REQUEST_METHOD=" + method);
	// line.append(" GATEWAY_INTERFACE=" + std::string("CGI/1.1"));
	line.append(" REDIRECT_STATUS=" + std::string("200"));
	line.append(" QUERY_STRING=" + confs[server_id].query_string);
	line.append(" SERVER_PROTOCOL=" + std::string("HTTP/1.1"));
	// line.append(" SERVER_SOFTWARE=" + std::string("WebServ"));
	line.append(" SCRIPT_NAME=" + php_path);//parse php_path
	line.append(" PATH_INFO=" + php_path);
	// line.append(" SERVER_NAME=");
	// if (confs[server_id].server_names.size() > 0)
	// 	line.append(confs[server_id].server_names[0]);
	// else
	// 	line.append(header.host);
	line.append(" SERVER_PORT=" + confs[server_id].listening.begin()->second[0]);
	line.append(" PATH_TRANSLATED=" + php_path);
	line.append(" REQUEST_URI=" + php_path + confs[server_id].query_string);
	line.append(" REMOTE_ADDR=" + header.host);
	line.append(" CONTENT_LENGTH=");
	if (header.content_length > 0)
		line.append(SSTR(header.content_length));

	line.append(" php-cgi php-files/test.php > /tmp/output_webserv.tmp");

	std::clog << line << std::endl;//*log
	return line;
}

void Server::php_cgi(struct header & header, id_server_type server_id, std::string php_path, std::string method){
	std::stringstream buffer_cout;
	std::streambuf *old_cout = std::cout.rdbuf(buffer_cout.rdbuf());


	system(cgi_vars(header, server_id, php_path, method).c_str());
	std::cout << std::ifstream("/tmp/output_webserv.tmp").rdbuf();
	remove("/tmp/output_webserv.tmp");

	std::cout.rdbuf(old_cout);

	// std::clog << buffer_cout.str() << std::endl;
	send_cgi(buffer_cout.str());
}