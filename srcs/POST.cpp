#include "../includes/includes.hpp"

bool    Server::parse_POST_request( std::list<std::string>::iterator it, std::list<std::string>::iterator ite )
{
	for ( ; it != ite ; it++ )
	{
		if (it->find("Content-Length:") != std::string::npos)
			p_POST.content_length = 0;
		else if (it->find("Content-Type:") != std::string::npos)
            p_POST.content_length = 0;
		if (it->find("{") != std::string::npos)
			while (it != ite)
			{
				if (it->find("}") == std::string::npos)
					break ;
				p_POST.content.append(*it);
			}
		if ( it == ite )
			break ;
	}
	return true;
}

bool    Server::treat_POST_request( const std::string & header )
{
	std::list<std::string> s_header = ft_split(header, "\n");
	parse_POST_request(s_header.begin(), s_header.end());

	std::cout << p_POST.content_length << std::endl;
	std::cout << p_POST.content_type << std::endl;
	std::cout << p_POST.content << std::endl;

	

	return true;
}