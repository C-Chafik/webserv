#include "includes.hpp"

void Server::redirect(std::string file, id_server_type server_id){

	if (file.find("/") != std::string::npos)
		file = file.substr(0, file.find("/"));

	std::clog << file << std::endl;
	if (confs[server_id].locations.find(file) != confs[server_id].locations.end()
		 && confs[server_id].locations[file].http_redirection.first == 301
		 && confs[server_id].locations[file].http_redirection.second != file){

		throw (Error_page(file));
	}

	if (confs[server_id].locations.find("/") != confs[server_id].locations.end()
		 && confs[server_id].locations["/"].http_redirection.first == 301
		 && confs[server_id].locations[file].http_redirection.second != file){

		throw (Error_page("/"));
	}
}
