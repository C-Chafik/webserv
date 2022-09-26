/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_handler.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmarouf <cmarouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/01 14:28:47 by cmarouf           #+#    #+#             */
/*   Updated: 2022/09/26 16:47:59 by cmarouf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HANDLER_HPP
# define REQUEST_HANDLER_HPP

# define BLACK   "\033[1m\033[30m"
# define RED     "\033[1m\033[31m"
# define GREEN   "\033[1m\033[32m"
# define YELLOW  "\033[1m\033[33m"
# define BLUE    "\033[1m\033[34m"
# define MAGENTA "\033[1m\033[35m"
# define CYAN    "\033[1m\033[36m"
# define WHITE   "\033[1m\033[37m"

# include <vector>
# include <list>
# include <iostream>
# include <string>
# include "../includes/includes.hpp"

struct request{
    std::string host;
    std::string port_host;
	std::string path;
	std::string header;
	int			method;
	int			content_length;
	int			read_content_length;
	bool 		full;

	request( void ) { method = 0; content_length = 0; read_content_length = 0; full = false; }
	~request( void ) {}
};

class request_handler
{
	public:
		request_handler( std::string & raw_header );
		~request_handler( void );

		bool 				state( void );
		std::string & 		get_header( void );
		struct request & 	get_request( void );
		int	 				which_method( void );
		
	private:
		struct	request				_request;
		int							_method;
		bool 						_state;
		std::vector<std::string> 	_data;
		std::string              	_header;

		void 	parse_header( void );
		void	retrieve_info( std::list<std::string>::iterator it, std::list<std::string>::iterator ite );
		void	assign_method( const std::string & method_name );
		void	assign_path( std::string & line );
		void	assign_host( std::string & line );

		void	print_all_informations( void );


		
		request_handler( void );
};

#endif