#include "includes.hpp"


short Server::host(in_addr_t ip_host, std::string name_host){
	//! change host var when we have the parse request's struct
	if ((parseG.ip_address == ip_host))
		return D_200;
	else{
		for (int i = 0; i < parseG.server_names.size(); i++)
			if (parseG.server_names[i] == name_host)
				return D_200;
	}
	std::cout << "No match" << std::endl;
	return D_400;
}

