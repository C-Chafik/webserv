/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmarouf <cmarouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/01 13:50:09 by cmarouf           #+#    #+#             */
/*   Updated: 2022/09/29 02:07:33 by cmarouf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Request.hpp"

Request::Request( void ) :  _read_content_length(0), _is_full(false), _start(false)
{
	
}

Request::~Request( void )
{

}

void Request::receive_request( int requestFd )
{
    char 			buffer[1024 + 1];
	size_t 			end;

	memset( buffer, 0, 1024 );
	if ( ( end = recv(requestFd, buffer, 1024 - 1, 0)) > 0 )
		insert(buffer, end);

	if ( check_if_header_is_received() == true )
		get_header_information();
	
	if ( is_full() == true )
		start_treating();
}

void		 		Request::start_treating( void )
{
	if ( _header.method == POST )
		get_body_information();
	else 
		return ;
}

bool	Request::check_if_header_is_received( void )
{
	if (_request.find("\r\n\r\n") != std::string::npos )
		return true;
	return false;
}

void	Request::insert( char * buffer, size_t len )
{
	_request.insert(_request.size(), buffer, len);

	if ( _start == true )
		_read_content_length += len;

	if ( _start == true )
		if ( _read_content_length >= _header.content_length )
			_is_full = true;
}

void	Request::get_body_information( void )
{
	size_t cursor = _request.find("\r\n\r\n");
	if ( cursor == std::string::npos )
	{
		_body.content = "";
		_body.length = 0;
		return ;
	}
	_body.content.insert(0, _request, cursor + 4, std::string::npos);
	_body.length = _body.content.size();
}

void	Request::get_header_information( void )
{
	std::string raw_header(_request);
	
	raw_header.resize(_request.find("\r\n\r\n"));

	std::stringstream header(raw_header);
	std::string buff;

	bool first = false;

	while ( std::getline(header, buff) )
	{
		std::list<std::string> infos = ft_split_no_r(buff, " \r");

		if ( infos.size() == 3 && (infos.front() == "GET" || infos.front() == "POST" || infos.front() == "DELETE") )
		{
			_header.method = assign_method(infos.front());
			infos.pop_back();
			_header.path = infos.back();
			first = true;
		}
		else if ( first == false )
		{
			std::cout << "THIS METHOD IS UNKNOWN ABORTING " << std::endl;
			_header.method = UNKNOWN;
			return ;
		}

		if ( infos.size() == 2 && infos.front() == "Host:" )
			assign_host(infos.back());

		if ( infos.size() == 2 && infos.front() == "Content-Length:" )
			_header.content_length = std::atoi(infos.back().c_str());
		
		if ( infos.size() == 2 && infos.front() == "Connection:" )
		{
			if ( infos.back() == "keep_alive")
				_header.keep_alive = true;
			else
				_header.keep_alive = false;
		}

		if ( infos.size() == 3 && infos.front() == "Content-Type:" )
		{
			infos.pop_front();
			_header.content_type = infos.front();
			_header.boundary = "--" + infos.back().substr(9);		

			_header.content_type.erase(_header.content_type.size() - 1);
		}
	}
	_start = true;
}

struct header & Request::get_header( void )
{
	return _header;
}

struct body & Request::get_body( void )
{
	return _body;
}

std::string & 	Request::get_request( void )
{
	return _request;
}

bool		 		Request::is_full( void )
{
	return _is_full;
}

int	Request::assign_method( const std::string & method_name )
{
	if ( method_name == "GET" )
		return GET;
	else if ( method_name == "POST" )
		return POST;
	else if ( method_name == "DELETE" )
		return DELETE;
	else
		return UNKNOWN;
}

void	Request::assign_host( std::string & line )
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

	_header.port_host = port;
	_header.host = ip_address;
}
