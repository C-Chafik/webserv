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

void Server::check_server_name(struct header & header, id_server_type &id){
	if (isIpAddress(header.host)){//if host is an ip
		if ((ipToHost(header.host) == ipToHost("localhost")))//check if the ip is the hostname
			if (id > 0)
				id = 0;//take first server config
		return;
	}
	else{//if host is an hostname
			   // std::vector<std::string>::iterator it = confs[id].server_names.begin();
			   // for (; it != confs[id].server_names.end(); it++)
			   //      if (*it == header.host)
			   //              return;
			   // if (id > 0)
			   //      id = 0;//take first server config
			   // return;
			   /**
				* find header.port dans tous confs
				* si le port est le meme check tous les server_names de tous les servers qui on le meme port
				* si trouve le server name, changer le server_id avec le bon 
				*/

				for (std::vector< struct config >::size_type k = 0; k < confs.size(); k++){
					   std::map< std::string, std::vector<std::string> >::iterator it = confs[k].listening.begin();
					   while (it != confs[k].listening.end()){
							   std::vector<std::string>::iterator it_port = std::find(it->second.begin(), it->second.end(), header.port_host);
							   if (it_port != it->second.end()){
									   std::cout <<  confs[k].server_names[0] << std::endl;
							   }
							   it++;
					   }
			   }
		}
}
