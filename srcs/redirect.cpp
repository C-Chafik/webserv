#include "includes.hpp"

void Server::redirect(std::string &file, id_server_type server_id){

	// std::cout << "file = " << file << std::endl;//*log
	// if (confs[server_id].locations.find(file) != confs[server_id].locations.end())//*log
	// 	std::cout << "redirecion = " << confs[server_id].locations[file].http_redirection.second << std::endl;//*log

	if (confs[server_id].locations.find(file) != confs[server_id].locations.end()
		 && confs[server_id].locations[file].http_redirection.first == 301
		 && confs[server_id].locations[file].http_redirection.second != file){
			
		// std::cout << "redirect file : " << file << " of server " << server_id << std::endl;//*log 
		throw (Error_page(file));
	}

	if (confs[server_id].locations.find("/") != confs[server_id].locations.end()
		 && confs[server_id].locations["/"].http_redirection.first == 301
		 && confs[server_id].locations[file].http_redirection.second != file){

		// std::cout << "redirect root : " << "/" << " of server " << server_id << std::endl;//*log 
		throw (Error_page("/"));
	}
}
