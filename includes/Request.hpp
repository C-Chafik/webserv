/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmarouf <cmarouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/01 14:28:47 by cmarouf           #+#    #+#             */
/*   Updated: 2022/10/08 15:17:38 by cmarouf          ###   ########.fr       */
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
	std::string header_str;
	std::string boundary;
	std::string content_type;
	std::string connection;
	int			method;
	long long			content_length;
	bool	 	keep_alive;
	int			clientFd;

	header(): content_type("text"), content_length(0), keep_alive(false) {}
};

struct body
{
	std::string body_path;
	
	std::string type;
	size_t		length;
};

class Request
{
	public:
		Request( void );
		Request( Request const & src );
		~Request( void );
		Request & operator=( Request const & src );
		
		void				search_tmp_name( void );

		void 				receive_request( int requestFd );
		void		 		start_treating( void );


		bool				check_if_header_is_received( void );
		void 				insert( char * buffer, size_t len, std::fstream & file );

		void		 		read_header( void );
		void		 		read_body( void );

		std::string &	 	get_file_path( void );
		struct header & 	get_header( void );
		std::string & 		get_request( void );
		struct body & 		get_body( void );
		bool		 		is_full( void );
		
	private:
		std::string		 			_tmp_filename;
		std::fstream			 	_request_fd;
		struct	header				_header;
		struct	body		 		_body;

		long long							_read_content_length;

		bool			 			_is_full;
		bool						_with_body;
		bool						_header_found;

		int		assign_method( const std::string & method_name );
		void	assign_host( std::string & line );
};

#endif