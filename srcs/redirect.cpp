#include "includes.hpp"

void Server::redirect(std::string &file, id_server_type server_id){

	/*LOGS*/
	std::cout << "file = " << file << std::endl;
	if (confs[server_id].locations.find(file) != confs[server_id].locations.end())
		std::cout << "redirecion = " << confs[server_id].locations[file].http_redirection.second << std::endl;
	/*END LOGS*/

	if (confs[server_id].locations.find(file) != confs[server_id].locations.end()
		 && confs[server_id].locations[file].http_redirection.first == 301
		 && confs[server_id].locations[file].http_redirection.second != file)
		 throw (Error_page(file));

	if (confs[server_id].locations.find("/") != confs[server_id].locations.end()
		 && confs[server_id].locations["/"].http_redirection.first == 301
		 && confs[server_id].locations[file].http_redirection.second != file)
		 throw (Error_page("/"));
}
