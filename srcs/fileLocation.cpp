#include "includes.hpp"

std::string Server::fileLocation(std::string request, std::vector< struct config >::size_type server_id){
	std::string::size_type slash;
	std::string location;
	std::string rtn;

	if (request.size() > 1 && (request.rfind("/") == request.size() - 1))
		request = request.substr(0, request.size() - 1);

	//find 1st location
	slash = request.find("/");
	if (!slash){
		request = request.substr(1, request.size());
		slash = request.find("/");
	} 

	if (slash != std::string::npos && request[slash + 1])
		location = request.substr(0, slash + 1);

	//check if config exist

	if (location.size() && confs[server_id].locations.find(location) != confs[server_id].locations.end()){//cas de dir/file.html
		if (confs[server_id].locations[location].root[confs[server_id].locations[location].root.size() - 1] != '/')
			confs[server_id].locations[location].root += "/";
		rtn.append(confs[server_id].locations[location].root);
		rtn.append(request.substr(slash + 1, request.size()));
	}
	else if (location.size() && confs[server_id].locations.find("/") != confs[server_id].locations.end()){//check if / config exist
		rtn.append(confs[server_id].locations["/"].root);
		if (confs[server_id].locations["/"].root[confs[server_id].locations["/"].root.size() - 1] != '/')
			confs[server_id].locations["/"].root += "/";
		rtn.append(request.substr(slash + 1, request.size()));
	}
	else if (confs[server_id].locations.find(request) != confs[server_id].locations.end()){//only file.html with config
		if (confs[server_id].locations[request].root[confs[server_id].locations[request].root.size() - 1] != '/')
			confs[server_id].locations[request].root += "/";
		rtn.append(confs[server_id].locations[request].root);
		rtn.append(request.substr(0, request.size()));
	}
	else if (confs[server_id].locations.find("/") != confs[server_id].locations.end()){//only file.html with "/" config
		if (confs[server_id].locations["/"].root[confs[server_id].locations["/"].root.size() - 1] != '/')
			confs[server_id].locations["/"].root += "/";
		rtn.append(confs[server_id].locations["/"].root);
		rtn.append(request.substr(0, request.size()));
	}
	else
		rtn.append(request);

	return rtn;
}

