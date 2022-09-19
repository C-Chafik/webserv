#ifndef PARSECONFIG_HPP
# define PARSECONFIG_HPP

#include "../includes/includes.hpp"


class parseConfig
{
	public:

	//* Contructor - Destructor

		parseConfig( std::string path );
		~parseConfig( void );

		bool    											fill_file( void );

		//? Parser
		void												parse_file( void );
		std::string 										insert_upload_path( std::string & line );
		std::pair<std::string, std::vector<std::string> >	insert_port( std::string raw_address );
		std::pair<int, std::string >						insert_http_redirection( std::string & raw_line );
		std::pair<std::vector<int>, std::string>			insert_error_page( std::string & raw_error_page );
		std::vector<std::string>							insert_server_names( std::string & raw_server_name );
		bool												insert_index( std::string & raw_index );
		void												insert_method( std::string & raw_method, const std::string & location );
		std::string		 	 		 	 					L_insert_root( std::string & line );
		std::string 										trim_data( std::string & raw_data, const std::string & data_name );
		std::list<std::string>::iterator		 			parse_location( std::list<std::string>::iterator it, std::list<std::string>::iterator ite );
		std::string	 	 		 	 						get_location_path( std::string & line );
		size_t												check_location( std::list<std::string>::iterator it, std::list<std::string>::iterator ite );
		int													insert_body_max_size( std::string & raw_data );
		int													count_server (std::list<std::string>::iterator it , std::list<std::string>::iterator ite );
		bool												search_informations( std::string & line );
		bool												check_closure( std::string & line );

		//? Exit
		int													exit_on_error( void );


		//? Utils
		std::list<std::string>								ft_split( const std::string & header, const std::string & charset );
		std::list<std::string>::iterator 					remove_empty_line( std::list<std::string> & container, std::list<std::string>::iterator & it );
		bool			 								 	exact_match( std::string & raw_str, const std::string & keyword );
		void    											remove_tab( std::string & str );
		void    											print_all_informations( void );		



		//* Getter

		bool 												state( void );
		size_t												get_server_number( void );
		std::vector< struct config > 						get_config( void );

	private:
		//! Parsing
		size_t					_closed;
		bool					_inside;
		std::list<std::string> 	_file;
		std::string		 		_file_path;
		struct parseLocation _parseLocation;
		struct config _config;
		std::vector< struct config > 			_configs;

		//! Exit Parsing
		std::string 			_actual_error;
		bool 					_state;


		parseConfig( void );
};

#endif