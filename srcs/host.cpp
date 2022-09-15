#include "includes.hpp"


bool Server::isIpAddress(std::string addr){
	if (!addr.empty()){
		size_t nb_bit_serie = 0;
		size_t point_location;

		point_location = addr.find(".");
		if (point_location == std::string::npos){
			return false;
		}

		int count = 0;
		while (count < addr.size() && nb_bit_serie < 3){
			if (addr[count] == '.'){
				++nb_bit_serie;
				if (nb_bit_serie != 3){
					point_location = addr.find(".", count + 1);
					if (point_location == std::string::npos)
						return false;
				}
			}else if (!isdigit(addr[count])){
				return false;
			}
			++count;
		}
		if (!addr[count] || ((addr.size()- 1) - count > 2))
			return false;
		for (; count < addr.size(); count++){
			if (!isdigit(addr[count]))
				return false;
		}
		return true;
	}
	return false;
}

bool Server::hostToIp(std::string host){
	if (isIpAddress(host){
		hostent* hostname = gethostbyname(host.c_str());
		std::string hostname_str = std::string(inet_ntoa(**(in_addr**)hostname->h_addr_list));
		std::cout << "HERE => " << hostname_str << std::endl;
		if(hostname_str == "127.0.0.1")
			return true;
		return false;
	}else {
		if (host != "127.0.0.1")
			return false;
		return true;
	}
}


void Server::check_host(){
	/* 
	TODO parse Host in request header and split ip and port if host is an ip
	*/
	parseG.ip_address = inet_addr("127.0.0.1");//*if no ip address for listen = 0
	parseG.server_names.push_back("localhost");//*if no server names just leave empty
	short send_type = host(inet_addr("127.0.0.1"), "localhost");

	hostToIp("localhost");

	switch (send_type)
	{
		case D_200:
			send_200("index.html");//!raw until parsing done (GET /file)
			// send_200("html_files/other.html");//print error
			// send_index();
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

