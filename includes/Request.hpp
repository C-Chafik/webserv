/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmarouf <cmarouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/01 14:28:47 by cmarouf           #+#    #+#             */
/*   Updated: 2022/09/29 01:56:36 by cmarouf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "includes.hpp"

struct header
{
    std::string host;
    std::string port_host;
	std::string path;
	std::string header;
	std::string boundary;
	std::string content_type;
	std::string connection;
	int			method;
	int			content_length;
	bool	 	keep_alive;
};

struct body
{
	std::string content;
	std::string type;
	size_t		length;
};

class Request
{
	public:
		Request( void );
		~Request( void );

		void 				receive_request( int requestFd );
		void		 		start_treating( void );


		bool				check_if_header_is_received( void );
		void 				insert( char * buffer, size_t len );

		void		 		get_header_information( void );
		void		 		get_body_information( void );

		struct header & 	get_header( void );
		std::string & 		get_request( void );
		struct body & 		get_body( void );
		bool		 		is_full( void );
		
	private:
		std::string					_request;
		struct	header				_header;
		struct	body		 		_body;

		int							_read_content_length;

		bool			 			_is_full;
		bool						_start;

		int		assign_method( const std::string & method_name );
		void	assign_host( std::string & line );
};

#endif