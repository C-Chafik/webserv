#include "../includes/includes.hpp"

T_POST::T_POST( void )
{

}

T_POST::~T_POST( void )
{

}

T_POST::T_POST( const std::string & header ) : _header(header)
{
	std::list<std::string> s_header = ft_split(_header, "\n");
	parse_POST_request(s_header.begin(), s_header.end());
}

bool    T_POST::parse_POST_request( std::list<std::string>::iterator it, std::list<std::string>::iterator ite )
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
	T_POST post(header);

	return true;
}