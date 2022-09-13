#include "includes.hpp"

std::string Server::fileLocation(std::string request){
	std::string::size_type slash;
	std::string location;
	std::string rtn;

	//find 1st location
	slash = request.find("/");
	if (slash != std::string::npos && request[slash + 1])
		location = request.substr(0, slash + 1);

	//check if config exist
	if (location.size() && parseG.location.find(location) != parseG.location.end()){//cas de dir/file.html
		rtn.append(parseG.location[location].root);
		rtn.append(request.substr(slash + 1, request.size()));
	}
	else if (location.size() && parseG.location.find("/") != parseG.location.end()){//check if / config exist
		rtn.append(parseG.location["/"].root);
		rtn.append(request.substr(slash + 1, request.size()));
	}
	else if (parseG.location.find(request) != parseG.location.end()){//only file.html with config
		rtn.append(parseG.location[request].root);
		rtn.append(request.substr(0, request.size()));
	}
	else if (parseG.location.find("/") != parseG.location.end()){//only file.html with "/" config
		rtn.append(parseG.location["/"].root);
		rtn.append(request.substr(0, request.size()));
	}
	else
		rtn.append(request);

	return rtn;
}

