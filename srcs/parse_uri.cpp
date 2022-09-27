#include "includes.hpp"


/*parse value/key in uri and return uri without this values*/
std::string Server::parse_uri(struct request &req, id_server_type server_id){
	typedef std::string::size_type s_t;
	
	std::string uri = req.path;
	s_t pos_i;
	std::string rtn;


	pos_i = uri.find("?");
	if (pos_i == std::string::npos)
		return req.path;

	rtn = req.path.substr(0, pos_i);

	s_t pos_end;

	/**
	 * @brief quoi faire si
	 * ?value=
	 * ?=key
	 * ?=
	 * ?value
	 */
	while ((pos_end = req.path.find("&", pos_i + 1)) != std::string::npos){
		std::string exp = req.path.substr(pos_i + 1, pos_end - (pos_i + 1));

		confs[server_id].get_vars[exp.substr(0, exp.find("="))] = exp.substr(exp.find("=") + 1, exp.size() - 1);

		pos_i = pos_end;
	}

	std::string exp = req.path.substr(pos_i + 1, req.path.size() - 1);
	confs[server_id].get_vars[exp.substr(0, exp.find("="))] = exp.substr(exp.find("=") + 1, exp.size() - 1);


	for (std::map<std::string, std::string>::iterator it = confs[server_id].get_vars.begin(); it != confs[server_id].get_vars.end(); it++)//*log for debug
	{
		std::cout << "key = " << it->first << std::endl;
		std::cout << "value = " << it->second << std::endl;
	}


	std::cout << rtn << std::endl;

	return rtn;
}