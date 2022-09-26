/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_handler.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmarouf <cmarouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/01 13:50:09 by cmarouf           #+#    #+#             */
/*   Updated: 2022/09/26 20:21:01 by cmarouf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.hpp"

//*? Ce qui nous interesse dans le header :
//*? Quel fichier est demander, donc verifier si ce fichier existe, OU est a executer avec la CGI
//*? La methode (GET, POST, ou DELETE)
//*? L'host, donc, l'adresse ip suivi du port, ou exclusivement localhost suivi du host (127.0.0.1:4242 OU localhost:4242 )
//*? La connection, doit etre keep-alive pour l'instant.
//*? sec-ch-ua (Le navigateur utiliser), doit etre Google Chrome

request_handler::request_handler( void )
{
	_method = GET;
	_state = true;
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

struct request & request_handler::get_request( void )
{
	return _request;
}

void	request_handler::print_all_informations( void )
{
	std::cout << GREEN << "REQUEST IP ADRESS : " << _request.host << std::endl;
	std::cout << GREEN << "REQUEST PORT : " << _request.port_host << std::endl;
	std::cout << GREEN << "REQUEST PATH" << "[" << _request.path << "]" << WHITE << std::endl;
}

void request_handler::parse_header( void )
{
	try
	{
		std::list<std::string>	s_header = ft_split(_header, "\n");
		retrieve_info(s_header.begin(), s_header.end());
	}
	catch ( std::bad_alloc & ba )
	{
    	std::cerr << "bad_alloc caught: " << ba.what() << '\n';
	}
}

void	request_handler::retrieve_info( std::list<std::string>::iterator it, std::list<std::string>::iterator ite )
{
	for ( ; it != ite ; it++ )
	{
		if ( it->find("GET") != std::string::npos )
		{
			assign_method("GET");
			assign_path(*it);
		}
		else if ( it->find("POST") != std::string::npos )
		{
			assign_method("POST");
			assign_path(*it);
		}
		else if ( it->find("DELETE") != std::string::npos )
		{
			assign_method("DELETE");
			assign_path(*it);
		}
		else if ( it->find("Host:") != std::string::npos )
			assign_host(*it);
		else if (it->find("Content-Length:") != std::string::npos)
			_content_length = std::atoi(insertion(*it, "Content-Length:").c_str());
	}
}

void	request_handler::assign_path( std::string & line )
{
	try
	{
		std::list<std::string> ret = ft_split(line, " ");
		ret.pop_back();
		_path.assign(ret.back());
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


void	request_handler::assign_host( std::string & line )
{
	std::string address;
	std::string port;
	std::string ip_address;

	address.assign(trim_data(line, "Host:"));
	if ( address.find(":") != std::string::npos )
	{
		ip_address = address.substr(0, address.find(":"));
		port = address.substr(address.find(":") + 1, std::string::npos );
	}
	else if ( address.find(".") != std::string::npos )
	{
		ip_address.assign(address);
		port.assign("8080");
	}
	else if ( address == "localhost" )
	{
		port.assign("8080");
		ip_address.assign("localhost");
	}
	else
	{
		port.assign(address);
		ip_address.assign("localhost");
	}

	_port_host = port;
	_host = ip_address;
}

void 		request_handler::append( char * buffer, size_t len )
{
	_header.append(buffer, len);
}

bool request_handler::check_if_header_is_received( void )
{
	if ( _header.find("\r\n\r\n") != std::string::npos )
		return true;
	return false;
}

struct request request_handler::gather_request_informations( void )
{
	struct request request;
	parse_header();

	request.host = _host;
	request.port_host = _port_host;
	request.path = _path;
	request.method = _method;
	request.content_length = _content_length;
	request.header = _header;

	return request;
}

void	Server::receive_request_body( struct request & req, int requestFd )
{
	char 		buffer[1024 + 1];
	size_t 		end;

	memset( buffer, 0, 1024 );
	if ( ( end = recv( requestFd, buffer, 1024 - 1, 0)) > 0 )
	{
		req.header.append(buffer, end);
		req.read_content_length += end;
	}
	if ( req.read_content_length >= req.content_length )
		req.full = true;
}

struct request Server::receive_request( int requestFd )
{
	request_handler header;
	struct request 	request;
    char 			buffer[1024 + 1];
	size_t 			end;

	memset( buffer, 0, 1024 );
	while ( ( end = recv(requestFd, buffer, 1024 - 1, 0)) > 0 )
	{
		header.append(buffer, end);
		if ( header.check_if_header_is_received() == true )
			break ;
	}
	request = header.gather_request_informations();
	
    return request;
}