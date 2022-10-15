/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmarouf <qatar75020@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/01 13:50:09 by cmarouf           #+#    #+#             */
/*   Updated: 2022/10/14 14:56:48 by cmarouf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Request.hpp"

Request::Request( void ) :  _read_content_length(0), _is_full(false), _with_body(false), _header_found(false)
{
	search_tmp_name();
}

Request::~Request( void )
{
	
}

Request::Request( Request const & src )
{
	*this = src;
}


Request & Request::operator=( Request const & src )
{
	_tmp_filename = src._tmp_filename;
	_read_content_length = src._read_content_length;	
	_is_full = src._is_full;
	_with_body = src._with_body;

	_header.host = src._header.host;
	_header.port_host = src._header.port_host;
	_header.path = src._header.path;
	_header.header_str = src._header.header_str;
	_header.boundary = src._header.boundary;
	_header.content_type = src._header.content_type;
	_header.connection = src._header.connection;
	_header.method = src._header.method;
	_header.content_length = src._header.content_length;
	_header.keep_alive = src._header.keep_alive;
	_body.body_path = src._body.body_path;
	_body.type = src._body.type;
	_body.length = src._body.length;

	return *this;
}

void	Request::search_tmp_name( void )
{
	std::stringstream 	out;
	std::string 		name = TMP_FILE_NAME;
	std::string 		s_id;
	size_t 				id = 174;
	
	while ( 1 )
	{
		out << id;
		s_id = out.str();
		
		if ( file_already_exist(name + s_id) == false )
		{
			_tmp_filename = name + s_id;
			return ;
		}
		
		s_id.clear();
		out.str("");
		id++;
	}
}

void Request::read_client( int requestFd )
{
    char 			buffer[8192 + 1];
	std::fstream	file( _tmp_filename.c_str(), std::fstream::app | std::fstream::binary );
	size_t 			end;

	memset( buffer, 0, 8192 );
	if ( ( end = recv(requestFd, buffer, 8192 - 1, 0)) > 0 )
		insert(buffer, end, file);
	if ( check_if_header_is_received() == true )
		read_header();
	file.close();
}

void	Request::insert( char * buffer, size_t len, std::fstream & file )
{
	file.write(buffer, len);
	
	// if ( _with_body == true )
	// {
		_read_content_length += len;
		if ( _with_body == true )
			// std::cout << RED << _header.content_length << WHITE << std::endl;
	// }

	if ( _with_body == true )
		if ( _read_content_length >= _header.content_length )
			_is_full = true;
	file.close();
}

bool	Request::check_if_header_is_received( void )
{
	std::string	 	buffer;
	std::string		full_buffer;
	std::ifstream 	tmp(_tmp_filename.c_str(), std::ifstream::binary );
	
	while ( std::getline(tmp, buffer) )
	{
		full_buffer += buffer;
		full_buffer += '\n';
		
		if ( full_buffer.rfind("\r\n\r\n") != std::string::npos )
		{
			// std::cout << YELLOW << full_buffer << WHITE << std::endl;
			return true;
		}
	}
	return false;
}

void	Request::read_body( void )
{
	
}

void	Request::read_header( void )
{
	std::ifstream tmp(_tmp_filename.c_str(), std::ifstream::binary );
	
	std::string buff;
	std::string end;

	bool first = false;

	while ( std::getline(tmp, buff) )
	{
		end += buff;
		end += '\n';
		if ( end.rfind("\r\n\r\n") != std::string::npos )
			break ;
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
			_header.method = UNKNOWN;
			return ;
		}

		if ( infos.size() == 2 && infos.front() == "Host:" )
			assign_host(infos.back());

		if ( infos.size() == 2 && infos.front() == "Content-Length:" ) //! If content_length is missing and the method is POST, must throw an 411
		{
			_with_body = true;
			_header.content_length = std::atoi(infos.back().c_str());
			if ( _read_content_length >= _header.content_length ) //? Here we check if we didnt get the full body while reading the header
				_is_full = true;
		}
		
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
			_header.raw_content_type = infos.front() + " " + infos.back();
			// std::clog << "[" << _header.raw_content_type << "]" << std::endl; 
			_header.boundary = "--" + infos.back().substr(9);		
			_header.content_type.erase(_header.content_type.size() - 1);
		}

		else if ( infos.size() == 2 && infos.front() == "Content-Type:" )
		{
			infos.pop_front();
			_header.content_type = infos.front();
			_header.raw_content_type = _header.content_type;
			// std::clog << "[" << _header.raw_content_type << "]" << std::endl; 	
		}
	}
}

struct header & Request::get_header( void )
{
	return _header;
}

struct body & Request::get_body( void )
{
	return _body;
}

bool		 		Request::is_full( void )
{
	return _is_full;
}

std::string &	 	Request::get_file_path( void )
{
	return _tmp_filename;
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
