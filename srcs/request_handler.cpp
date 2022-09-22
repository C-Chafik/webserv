/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_handler.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmarouf <qatar75020@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/01 13:50:09 by cmarouf           #+#    #+#             */
/*   Updated: 2022/09/21 18:31:21 by cmarouf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.hpp"

//*? Ce qui nous interesse dans le header :
//*? Quel fichier est demander, donc verifier si ce fichier existe, OU est a executer avec la CGI
//*? La methode (GET, POST, ou DELETE)
//*? L'host, donc, l'adresse ip suivi du port, ou exclusivement localhost suivi du host (127.0.0.1:4242 OU localhost:4242 )
//*? La connection, doit etre keep-alive pour l'instant.
//*? sec-ch-ua (Le navigateur utiliser), doit etre Google Chrome

request_handler::request_handler( void ) : _state(false)
{

}

request_handler::request_handler( char * raw_header ) : _state(false), _header(raw_header)
{
	_state = true;
	
	if ( _header.empty() == false )
		parse_header();
}

request_handler::~request_handler( void )
{

}

std::string & request_handler::get_header( void )
{
	return _header;
}

int		request_handler::which_method( void )
{
	return _method;
}

bool	request_handler::state( void )
{
	return _state;
}

void request_handler::parse_header( void )
{
	try
	{
		std::list<std::string>	s_header = ft_split(_header, "\n");
		retrieve_method(s_header.begin(), s_header.end());
	}
	catch ( std::bad_alloc & ba )
	{
    	std::cerr << "bad_alloc caught: " << ba.what() << '\n';
	}
}

void	request_handler::assign_method( const std::string & method_name )
{
	if ( method_name == "GET" )
		_method = GET;
	else if ( method_name == "POST" )
		_method = POST;
	else if ( method_name == "DELETE" )
		_method = DELETE;
	else
		_method = UNKNOWN;
}

void	request_handler::retrieve_method( std::list<std::string>::iterator it, std::list<std::string>::iterator ite )
{
	for ( ; it != ite ; it++ )
	{
		if ( it->find("GET") != std::string::npos )
			return assign_method("GET");
		else if ( it->find("POST") != std::string::npos )
			return assign_method("POST");
		else if ( it->find("DELETE") != std::string::npos )
			return assign_method("DELETE");
	}
	return assign_method("UNKOWN");
}










int Server::treat_request( int requestFd )
{
    char header[4096 + 1];
	size_t end;

	memset(header, 0, 4096);
	while ( (end = recv(requestFd, header, 4096 - 1, 0)) > 0 )
	{
		if ( header[end - 1] == '\n' )
			break ;
		memset(header, 0, 4096);
	}
	if ( end == static_cast<size_t>(-1) )
	{
		std::cout << "ERROR RECEIVING THE HEADER " << std::endl;
		exit(0);
	}
	std::cout << YELLOW << header << WHITE << std::endl;

    request_handler request(header);
	if ( request.state() == false )
	{
		std::cout << RED << "SOMETHING WENT WRONG IN THE REQUEST HEADER " << std::endl;
		exit(0);
	}

	_header = request.get_header();

    return request.which_method();
}