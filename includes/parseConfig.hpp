#ifndef PARSECONFIG_HPP
# define PARSECONFIG_HPP

#include "../includes/includes.hpp"


class parseConfig
{
	public:

		//? Contructor - Destructor
		parseConfig( std::string path = std::string("conf/default.conf") );
		~parseConfig( void );


		//? Exit
		int													exit_on_error( void );

		//? Getter
		bool 												state( void );
		size_t												get_server_number( void );
		std::vector< struct config > 						get_config( void );




	private:
		//! Parsing
		size_t					_closed;
		bool					_inside;
		std::list<std::string> 	_file;
		std::string		 		_file_path;

		//! Data
		struct parseLocation _parseLocation;
		struct config _config;
		std::vector< struct config > 			_configs;

		//! Exit Parsing
		std::string 			_actual_error;
		bool 					_state;


		//? Main
		bool    											fill_file( void );
		void												parse_file( void );
		void												set_default_value( void );

		//? Insertion
		std::string 										insert_upload_path( std::string & line );
		std::pair<std::string, std::vector<std::string> >	insert_port( std::string raw_address );
		std::pair<int, std::string >						insert_http_redirection( std::string & raw_line );
		std::pair<std::vector<int>, std::string>			insert_error_page( std::string & raw_error_page );
		std::vector<std::string>							insert_server_names( std::string & raw_server_name );
		bool												insert_index( std::string & raw_index );
		void												insert_method( std::string & raw_method, const std::string & location );
		int													insert_body_max_size( std::string & raw_data );
		std::string		 	 		 	 					insert_root( std::string & line );


		//? Utils
		std::string 										trim_data( std::string & raw_data, const std::string & data_name );
		std::list<std::string>::iterator		 			parse_location( std::list<std::string>::iterator it, std::list<std::string>::iterator ite );
		std::string	 	 		 	 						get_location_path( std::string & line );
		size_t												check_location( std::list<std::string>::iterator it, std::list<std::string>::iterator ite );
		int													count_server (std::list<std::string>::iterator it , std::list<std::string>::iterator ite );
		bool												search_informations( std::string & line );
		bool												check_closure( std::string & line );
		std::list<std::string>::iterator 					remove_empty_line( std::list<std::string> & container, std::list<std::string>::iterator & it );
		bool			 								 	exact_match( std::string & raw_str, const std::string & keyword );
		void    											remove_tab( std::string & str );
		void												parsing_error( const std::string & error, const std::string & where = std::string() );
		void    											print_all_informations( void );	


		//? Common
		std::list<std::string>								ft_split( const std::string & header, const std::string & charset );	

		//? Default Constructor
		parseConfig( void );
};

#endif