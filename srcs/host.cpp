#include "includes.hpp"

void Server::check_host(){
	/* 
	TODO parse Host in request header and split ip and port if host is an ip
	*/
	parseG.ip_address = inet_addr("127.0.0.1");//*if no ip address for listen = 0
	parseG.server_names.push_back("localhost");//*if no server names just leave empty
	short send_type = host(inet_addr("127.0.0.1"), "localhost");

	switch (send_type)
	{
		case D_200:
			// send_200("html_files/test.html");//!raw until parsing done (GET /file)
			send_200("html_files/other.html");//print error
			break;
		
		case D_400:
			send_400();
			break;

		default:
			break;
	}
}


short Server::host(in_addr_t ip_host, std::string name_host){
	//! change host var when we have the parse request's struct
	if (ip_host && (parseG.ip_address == ip_host))
		return D_200;
	else
		for (int i = 0; i < parseG.server_names.size(); i++)
			if (parseG.server_names[i] == name_host)
				return D_200;

	return D_400;
}

