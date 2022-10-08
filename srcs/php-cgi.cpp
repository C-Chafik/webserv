#include "includes.hpp"

std::string Server::cgi_vars(struct header & header, id_server_type server_id, std::string php_path, std::string method, struct body& body){
	std::string line;

	line.append("CONTENT_TYPE=" + header.content_type);
	line.append(" REDIRECT_STATUS=");
	(void) method;
	line.append(" REQUEST_METHOD=" + method);
	line.append(" GATEWAY_INTERFACE=" + std::string("CGI/1.1"));
	line.append(" REDIRECT_STATUS=" + std::string("200"));
	line.append(" QUERY_STRING=" + confs[server_id].query_string);
	line.append(" SERVER_PROTOCOL=" + std::string("HTTP/1.1"));
	line.append(" SERVER_SOFTWARE=" + std::string("WebServ"));
	line.append(" SCRIPT_NAME=" + php_path);//parse php_path
	line.append(" PATH_INFO=" + php_path);
	line.append(" SERVER_NAME=");
	if (confs[server_id].server_names.size() > 0)
		line.append(confs[server_id].server_names[0]);
	else
		line.append(header.host);
	line.append(" SERVER_PORT=" + confs[server_id].listening.begin()->second[0]);
	line.append(" PATH_TRANSLATED=" + php_path);
	line.append(" REQUEST_URI=" + php_path + confs[server_id].query_string);
	line.append(" REMOTE_ADDR=" + header.host);
	line.append(" CONTENT_LENGTH=");
	if (header.content_length > 0)
		line.append(SSTR(header.content_length));

	if (method == "POST"){
		line.append(" < ");
		line.append(body.body_path);
	}
	line.append(" php-cgi ");
	line.append(php_path);
	line.append(" > /tmp/output_webserv.tmp");

	// std::clog << line << std::endl;
	return line;
}

std::string Server::parseCgiHeader(std::string buffer){
	HGen.clear();

	if (buffer.find("\r\n\r\n") == std::string::npos)
		return buffer;

	std::string rtn;
	std::string header;
	std::string CT = "Content-type: ";
	std::string S = "Status: ";
	std::string LOC = "Location: ";
	std::string::size_type st;
	
	header = buffer.substr(0, buffer.find("\r\n\r\n"));
	rtn = buffer.substr(header.size() + 4, buffer.size());

	if ( (st = header.find(CT)) != std::string::npos )
		HGen.setType(header.substr(st + CT.size(), header.find_first_of("\r", st) - CT.size()));

	if ( (st = header.find(S)) != std::string::npos )
		HGen.setStatus(header.substr(st + S.size(), header.find_first_of("\r", st) - S.size()));

	if ( (st = header.find(LOC)) != std::string::npos ){
		HGen.setLocation(header.substr(st + LOC.size(), header.find_first_of("\r", st) - LOC.size()));
		HGen.processResponse();
	}

	// std::clog << HGen.getStr() << std::endl;
	return rtn;
}

bool Server::cgi_error(id_server_type server_id){
	if (HGen.getStatus().find("200") != std::string::npos || HGen.checkStatus())
		return false;
	if (HGen.getStatus().find("500") != std::string::npos)
		send_500(server_id);
	else if (HGen.getStatus().find("400") != std::string::npos)
		send_400(server_id);
	else if (HGen.getStatus().find("404") != std::string::npos)
		send_404(server_id);

	return true;
}

void Server::php_cgi(struct header & header, id_server_type server_id, std::string php_path, std::string method, struct body& body){
	std::stringstream buffer_cout;
	std::streambuf *old_cout = std::cout.rdbuf(buffer_cout.rdbuf());


	system(cgi_vars(header, server_id, php_path, method, body).c_str());
	std::cout << std::ifstream("/tmp/output_webserv.tmp").rdbuf();
	remove("/tmp/output_webserv.tmp");

	std::cout.rdbuf(old_cout);

	send_cgi(server_id, parseCgiHeader(buffer_cout.str()));
}