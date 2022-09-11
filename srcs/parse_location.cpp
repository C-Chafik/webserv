#include "includes.hpp"

std::string Server::fileLocation(std::string request){
	std::string rtn;
	std::string location;

	std::cout << "REQUEST : " << request << std::endl;
	std::cout << "location : " << location << std::endl;
	std::cout << "rtn : " << rtn << std::endl;
	
	if (request.find(std::string("/")) != std::string::npos)
		location = request.substr(0, request.find(std::string("/")));
	std::cout << "location : " << location << std::endl;

	if (location.size() && parseG.location.find(location) != parseG.location.end()){
		std::cout << "if" << std::endl;
		rtn.append(parseG.location[location].root);
		rtn.append(request.substr(request.find(std::string("/")) + 1, request[request.size() - 1]));
	}
	else if (parseG.location.find("/") != parseG.location.end()){
		std::cout << "else if" << std::endl;
		rtn.append(parseG.location["/"].root);
	}

	std::cout << "rtn4 : " << rtn << std::endl;
	if (request.find(std::string("/")) != std::string::npos)
		rtn.append(parseG.location[location].root);
	std::cout << "rtn5 : " << rtn << std::endl;

	rtn.append(request);
	std::cout << "rtn6 : " << rtn << std::endl;

	std::cout << rtn << std::endl;
	return rtn;
}
