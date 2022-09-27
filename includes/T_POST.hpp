#ifndef T_POST_HPP
# define T_POST_HPP

# include "includes.hpp"

class T_POST
{
	public:
		T_POST( const std::string & req, const std::string & path  );
		~T_POST( void );

		bool	check_keep_connection( void );

	private:
		std::string			_request;

		//! Header informations
		size_t      		_content_length;
		std::string			_uploading_path;
		std::string 		_content_type;
		std::string	 		_boundary;
		std::string 		_connection;
		std::string 		_referer;
		bool		 		_keep_alive;

		//! Body informations
		std::string 		_content;

		void	print_all_informations( void );

		std::string  get_boundary( std::string & line );
		std::string get_file_name( const std::string & line );
		bool	get_header_informations( std::list<std::string>::iterator it, std::list<std::string>::iterator ite );
		bool	treat_content( const std::string & encoding );
		bool	get_content( std::string & req );

		bool	retrieve_file_content( std::string & s_request );

		//?  Retrieve Content, encoded, and not encoded version
		bool            upload_file( const std::string & data, const std::string & file_name );
		// bool            store_form( std::list<std::string>::iterator it, std::list<std::string>::iterator ite )
		

		T_POST( void );
};

#endif