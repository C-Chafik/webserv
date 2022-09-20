#include "includes.hpp"

std::string Server::fileLocation(std::string request, std::vector< struct config >::size_type serverNb){
	std::string::size_type slash;
	std::string location;
	std::string rtn;

	//find 1st location
	slash = request.find("/");
	if (slash != std::string::npos && request[slash + 1])
		location = request.substr(0, slash + 1);

	//check if config exist
	if (location.size() && confs[serverNb].locations.find(location) != confs[serverNb].locations.end()){//cas de dir/file.html
		rtn.append(confs[serverNb].locations[location].root);
		rtn.append(request.substr(slash + 1, request.size()));
	}
	else if (location.size() && confs[serverNb].locations.find("/") != confs[serverNb].locations.end()){//check if / config exist
		rtn.append(confs[serverNb].locations["/"].root);
		rtn.append(request.substr(slash + 1, request.size()));
	}
	else if (confs[serverNb].locations.find(request) != confs[serverNb].locations.end()){//only file.html with config
		rtn.append(confs[serverNb].locations[request].root);
		rtn.append(request.substr(0, request.size()));
	}
	else if (confs[serverNb].locations.find("/") != confs[serverNb].locations.end()){//only file.html with "/" config
		rtn.append(confs[serverNb].locations["/"].root);
		rtn.append(request.substr(0, request.size()));
	}
	else
		rtn.append(request);

	std::cout << rtn << std::endl;
	return rtn;
}

