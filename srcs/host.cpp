#include "includes.hpp"

/*true if string is an ip address format*/
bool Server::isIpAddress(std::string addr){
	if (!addr.empty()){
		size_t nb_bit_serie = 0;
		size_t point_location;

		point_location = addr.find(".");
		if (point_location == std::string::npos){
			return false;
		}

		std::string::size_type count = 0;
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

/*return true if the string is or is an hostname wich is 127.0.0.1*/
bool Server::hostToIp(std::string host){
	if (!isIpAddress(host)){
		hostent* hostname = gethostbyname(host.c_str());
		std::string hostname_str = std::string(inet_ntoa(**(in_addr**)hostname->h_addr_list));
		if(hostname_str == "127.0.0.1")
			return true;
		return false;
	}else {
		if (host != "127.0.0.1")
			return false;
		return true;
	}
}

std::string Server::ipToHost(std::string host){
	hostent* hostname = gethostbyname(host.c_str());
	std::string hostname_str = std::string(inet_ntoa(**(in_addr**)hostname->h_addr_list));
	return hostname_str;
}

void Server::check_server_name(struct request &req, id_server_type &id){
	if (isIpAddress(req.host)){//if host is an ip
		if ((ipToHost(req.host) == ipToHost("localhost")))//check if the ip is the hostname
			if (id > 0)
				id = 0;//take first server config
		return;
	}
	else{//if host is an hostname
		std::vector<std::string>::iterator it = confs[id].server_names.begin();
		for (; it != confs[id].server_names.end(); it++)
			if (*it == req.host)
				return;
		if (id > 0)
			id = 0;//take first server config
		return;
	}
}
