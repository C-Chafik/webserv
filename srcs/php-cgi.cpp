#include "includes.hpp"

void Server::php_cgi(id_server_type server_id){
	std::string line("name=Hugo");

	line.append(" QUERY_STRING=");
	line.append(confs[server_id].query_string);

	
	line.append(" /usr/bin/php-cgi -f php-files/test.php");
	system(line.c_str());
}