#include "../includes/parseConfig.hpp"

/////////////////////////////////////////////////////
///                                               ///
///          CONSTRUCTOR - DESTRUCTOR             ///
///                                               ///
/////////////////////////////////////////////////////

parseConfig::parseConfig( void )
{

}

parseConfig::~parseConfig( void )
{

}

parseConfig::parseConfig( std::string path ) : _file_path(path)
{
	_closed = 0;
	_inside = 0;
	_state = true;

	if ( fill_file() == false )
		return ;

	parse_file();
}

/////////////////////////////////////////////////////
///                                               ///
///          CONSTRUCTOR - DESTRUCTOR             ///
///                                               ///
/////////////////////////////////////////////////////

bool parseConfig::check_conf_name( std::string & str )
{
	std::string::size_type end = str.size();

	if ( end <= 5 ) //? yes i refuse a file named ".conf"
		return false;

	if ( str[end - 1] == '/' || str[end - 1] == '\\' )
		return false;
	
	else if ( str[end - 1] != 'f' )
		return false;
	
	else if ( str[end - 2] != 'n' )
		return false;
	
	else if ( str[end - 3] != 'o' )
		return false;
	
	else if ( str[end - 4] != 'c' )
		return false;
	
	else if ( str[end - 5] != '.' )
		return false;

	return true;
}

bool parseConfig::fill_file( void )
{
	std::ifstream 	file;
	std::string 	buffer;
	std::string		fileSTR;

std::cout << "wtf" <<std::endl;

	if ( check_conf_name( _file_path ) == false )
	{
		parsing_error("ERROR IN THE FILE NAME ");
		return false;
	}

	std::cout << "wtf" <<std::endl;

	file.open(_file_path.c_str());
	if (!file.is_open())
	{
		parsing_error("ERROR OPENING THE FILE");
		return false;
	}

	std::cout << "wtf" <<std::endl;

	while ( getline(file, buffer, '\n') )
	{
		fileSTR += buffer;
		fileSTR += "\n";
	}

	std::cout << "wtf" <<std::endl;

	std::cout << "[" << fileSTR << "]";

		std::cout << "wtf" <<std::endl;

	_file = ft_split(fileSTR, "\n");

	for ( std::list<std::string>::iterator it = _file.begin() ; it != _file.end() ; it++ )
		remove_tab(*it);

	for ( std::list<std::string>::iterator it = _file.begin() ; it != _file.end() ; it++ )
		if (it->empty())
			it = remove_empty_line(_file, it);

	return true;
}


/////////////////////////////////////////////////////
///                                               ///
///          			EXIT            		  ///
///                                               ///
/////////////////////////////////////////////////////

int parseConfig::exit_on_error ( void )
{
	std::cout << RED << _actual_error << std::endl;
	return 1;
}

/////////////////////////////////////////////////////
///                                               ///
///          			EXIT            		  ///
///                                               ///
/////////////////////////////////////////////////////


/////////////////////////////////////////////////////
///                                               ///
///          			PARSER            		  ///
///                                               ///
/////////////////////////////////////////////////////


//! Must be a copy, not a value (Can change it later)
std::list<std::string>::iterator	parseConfig::parse_location( std::list<std::string>::iterator it, std::list<std::string>::iterator ite )
{
	std::string 	path;
	int 			closed = 0;
	int	 			inside = 0;

	path = get_location_path(*it);
	if ( path.empty() )
	{
		parsing_error("ERROR IN A LOCATION PATH : ", *it);
		return _file.end();
	}

	if ( check_location(it, ite) == std::string::npos )
		return _file.end();

	for ( ; it != ite ; it++ )
	{
		if ( (it->find("}") != std::string::npos) && closed == 1)
		{
			it++;
			break ;
		}
		if ( it->find(";") == std::string::npos && inside == 1 && closed == 1 )
		{
			parsing_error("MISSING ; : ", *it);
			return _file.end();
		}
		if ( it->find("{") != std::string::npos )
		{
			inside = 1;
			closed = 1;
		}

		if ( exact_match(*it, "root") == true )
			_config.locations[path].root = insert_root(*it);

		else if ( exact_match(*it, "autoindex") == true )
			_config.locations[path].autoindex = insert_autoindex(*it);

		else if ( exact_match(*it, "return") == true )
			_config.locations[path].http_redirection = insert_http_redirection(*it);

		else if ( exact_match(*it, "method_accept") == true )
			insert_method(*it, path);

		else if ( exact_match(*it, "set_upload") == true )
		{
			std::string ret = insert_upload_path(*it);
			if ( ret.empty() )
			{
				parsing_error("UPLOAD PATH IS INVALID : ", *it);
				return _file.end();
			}
			_config.locations[path].upload_path = ret;
		}

		else if ( exact_match(*it, "client_max_body_size") == true )
		{
			_config.locations[path].body_max_size = insert_body_max_size(*it);
			if ( _config.locations[path].body_max_size == -1 )
			{
				parsing_error("ERROR AT : ", *it);
				return _file.end();
			}
		}

	}

	return it;
}

bool parseConfig::search_informations( std::string & line )
{
		if ( exact_match(line, "listen") == true )
		{
			std::pair<std::map<std::string, std::vector<std::string> >::iterator, bool > ret;
			std::pair<std::string, std::vector<std::string> > listen(insert_port(line));

			ret = _config.listening.insert(listen);
			if ( ret.second == false )
				ret.first->second.push_back(listen.second.front());
		}

		else if ( exact_match(line, "server_name") == true )
			_config.server_names = insert_server_names(line);

		else if ( exact_match(line, "client_max_body_size") == true )
		{
			_config.locations["/"].body_max_size = insert_body_max_size(line);
			if ( _config.locations["/"].body_max_size == -1 )
			{
				parsing_error("ERROR AT : ", line);
				return false;
			}
		}

		else if ( exact_match(line, "root") == true )
			_config.locations["/"].root = insert_root(line);

		else if ( exact_match(line, "index") == true )
		{
			std::string ret = insert_index(line);
			if ( ret.empty() )
			{
				parsing_error(" : ", line);
				return false;
			}
			_config.index = ret;
		}

		else if ( exact_match(line, "error_page") == true )
			_config.errors.push_back(insert_error_page(line));

		else if ( exact_match(line, "autoindex") == true )
			_config.locations["/"].autoindex = insert_autoindex(line);
		
		else if ( exact_match(line, "return") == true )
			_config.locations["/"].http_redirection = insert_http_redirection(line);
		
		else if ( exact_match(line, "method_accept") == true )
			insert_method(line, "/");
		
		else if ( exact_match(line, "set_upload") == true )
		{
			std::string ret = insert_upload_path(line);
			if ( ret.empty() )
			{
				parsing_error("UPLOAD PATH IS INVALID : ", line);
				return false;
			}
			_config.locations["/"].upload_path = ret;
		}
	
		else if ( line != *(_file.begin()) && line.find_first_of("{}") == std::string::npos )
		{
			parsing_error("UNKNOWN COMMAND : ", line);
			return false;
		}
	
	return true;
}

void	parseConfig::parse_file( void )
{
	std::list<std::string>::iterator it = _file.begin();
	std::list<std::string>::iterator ite = _file.end();

	while (it->empty())
		it++;
	
	for ( ; it != ite ; it++ )
	{
		if ( it->find("server") != std::string::npos )
		{
			while ( it != ite )
			{
				if ( ( it->find("}") != std::string::npos ) && _closed == 1 && it->find("location") == std::string::npos )
				{
					if ( it->at(it->length() - 1) != '}' )
						return parsing_error("INVALID CLOSURE ");
					break ;
				}

				if ( check_closure(*it) == false )
					return ;

				if ( _closed == 0 )
					return parsing_error("MISSING OPENING BRACE");

				if ( exact_match(*it, "location") == true )
				{
					it = parse_location(it, ite);
					if ( it == _file.end() )
						return ;
					continue ;
				}

				else if ( search_informations(*it) == false )
					return ;

				it++;
			}
		}
		if ( _config.listening.empty() )
				_config.listening.insert(insert_port("localhost:8080"));
		_configs.push_back(_config);
		_config.clear();
		_inside = 0;
		_closed = 0;
		
		if ( it == ite )
			break ;
	}
	// print_all_informations();
}

/////////////////////////////////////////////////////
///                                               ///
///          			PARSER            		  ///
///                                               ///
/////////////////////////////////////////////////////



/////////////////////////////////////////////////////
///                                               ///
///          			GETTER            		  ///
///                                               ///
/////////////////////////////////////////////////////

bool	parseConfig::state( void )
{
	return _state;
}

size_t parseConfig::get_server_number( void )
{
	return count_server(_file.begin(), _file.end());
}

std::vector< struct config > parseConfig::get_config( void )
{
	return _configs;
}

/////////////////////////////////////////////////////
///                                               ///
///          			GETTER            		  ///
///                                               ///
/////////////////////////////////////////////////////