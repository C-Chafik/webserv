#include "includes.hpp"

std::string Server::fileLocation(std::string request){
	std::string rtn;
	std::string location = request.substr(0, request.find(std::string("/")));

	if (parseG.location.find("html_files") != parseG.location.end()){
		rtn.append(parseG.location["html_files"].root);
		rtn.append(request += location.size());
	}

	return rtn;
}