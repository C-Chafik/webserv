/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_handler.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmarouf <qatar75020@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/01 13:50:09 by cmarouf           #+#    #+#             */
/*   Updated: 2022/09/02 17:07:56 by cmarouf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.hpp"

//*? Ce qui nous interesse dans le header :
//*? Quel fichier est demander, donc verifier si ce fichier existe, OU est a executer avec la CGI
//*? La methode (GET, POST, ou DELETE)
//*? L'host, donc, l'adresse ip suivi du port, ou exclusivement localhost suivi du host (127.0.0.1:4242 OU localhost:4242 )
//*? La connection, doit etre keep-alive pour l'instant.
//*? sec-ch-ua (Le navigateur utiliser), doit etre Google Chrome

request_handler::request_handler( void ) : state(true), _data()
{

}

request_handler::request_handler( char * raw_header ) : _header(raw_header), state(true), _data()
{
	
}

request_handler::~request_handler( void )
{

}

std::list<std::string> request_handler::ft_split(std::string header, std::string charset )
{
	size_t start = 0, end, charset_len = charset.length();
	std::string tmp;
	std::list<std::string> ret;
	while ((end = header.find(charset, start)) != std::string::npos)
	{
		tmp = header.substr(start, 	end - start);
		start = end + charset_len;
		ret.push_back(tmp);
	}
	ret.push_back(header.substr(start));
	return ret;
}

void request_handler::parse_header( void )
{
	std::list<std::string> s_header = ft_split(_header, "\n");
	std::string tmp;
	std::string first = s_header.front();

	//! First line is very specific since there is 3 crucial informations in one line
	// std::cout << BLUE << first << WHITE << std::endl;
	for ( size_t i = 0 ; first[i] != '\0' ; i++ )
	{
		if ( first[i] == ' ' )
		{
			i++;
			_data.push_back(tmp);
			tmp.clear();
		}
		tmp.append(1, first[i]);
	}
	_data.push_back(tmp);
	
	// std::vector<std::string>::iterator it = _data.begin();
	// for ( ; it != _data.end(); it++ )
	// {
	// 	std::cout << *it << std::endl;
	// }
	return ;
}