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


bool parseConfig::fill_file( void )
{
	std::ifstream file;
	std::string buffer;
	std::string	fileSTR;

	file.open(_file_path.c_str());
	if (!file.is_open())
	{
		_actual_error = "ERROR OPENING THE FILE";
		return false;
	}

	while (getline(file, buffer, '\n'))
	{
		fileSTR += buffer;
		fileSTR += "\n";
	}

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
///          			UTILS             		  ///
///                                               ///
/////////////////////////////////////////////////////

std::list<std::string> parseConfig::ft_split( const std::string & header, const std::string & charset )
{
	size_t 					start = 0, end, charset_len = charset.length();
	std::string				tmp;
	std::list<std::string> 	ret;

	while ((end = header.find(charset, start)) != std::string::npos)
	{
		tmp = header.substr(start, 	end - start);
		start = end + charset_len;
		ret.push_back(tmp);
	}
	ret.push_back(header.substr(start));

	return ret;		
}

std::list<std::string>::iterator parseConfig::remove_empty_line( std::list<std::string> & container, std::list<std::string>::iterator & it )
{
	if (it->empty())
		container.erase(it);
	
	std::list<std::string>::iterator new_it = container.begin();

	return new_it;
}

void	parseConfig::remove_tab( std::string & str )
{
	for ( std::string::size_type i = 0 ; str[i] == '\t' ; i++ )
		str.erase(i, 1);
}

void	parseConfig::print_all_informations( void )
{
	int i = 0;
	std::cout << MAGENTA << "PRINTING ALL GATHERED INFORMATIONS " << std::endl << std::endl;
	for ( std::vector< struct config >::iterator conf_it = _configs.begin() ; conf_it != _configs.end() ; conf_it++ )
	{
		std::cout << "PRINTING CONFIG NUMBER : " << i << std::endl; 

		std::map<std::string, std::vector<std::string> >::iterator mit = conf_it->listening.begin();
		std::map<std::string, std::vector<std::string> >::iterator mite = conf_it->listening.end();

		std::vector<std::string>::iterator vit = conf_it->server_names.begin();
		std::vector<std::string>::iterator vite = conf_it->server_names.end();

		std::vector< std::pair<std::vector<int>, std::string> >::iterator eit = conf_it->errors.begin();
		std::vector< std::pair<std::vector<int>, std::string> >::iterator eite = conf_it->errors.end();

		std::map< std::string, struct parseLocation >::iterator lit = conf_it->locations.begin();
		std::map< std::string, struct parseLocation >::iterator lite = conf_it->locations.end();


		std::cout << "ALL LISTEN PORT AND IP " << std::endl;
		for ( ; mit != mite ; mit++ )
		{
			std::cout << mit->first << " ";
			for ( std::vector<std::string>::iterator it = mit->second.begin() ; it != mit->second.end() ; it++ )
				std::cout << *it <<" : ";
			std::cout << std::endl;
		}
		std::cout << std::endl;
		std::cout << "ALL SERVER NAMES " << std::endl;
		for ( ; vit != vite ; vit++ )
			std::cout << *vit << " ";
		std::cout << std::endl;
		std::cout << "CLIENT_MAX_BODY_SIZE " << std::endl;
		std::cout << conf_it->body_max_size << std::endl;
		std::cout << "ALL ERRORS PAGE" << std::endl;
		for ( ; eit != eite ; eit++ )
		{
			std::cout << "ERROR PATH : " << eit->second << " FOR ERROR CODE -> ";
			for ( std::vector<int>::iterator it = eit->first.begin() ; it != eit->first.end() ; it++ )
				std::cout << *it <<": ";
			std::cout << std::endl;
		}

		std::cout << "ALL LOCATIONS INFORMATIONS" << std::endl;
		for ( ; lit != lite ; lit++)
		{
			std::cout << " FOR THE PATH : " << lit->first << std::endl;
			std::cout << " ROOT -> " << lit->second.root << std::endl;
			std::cout << " REPOSITORY LISTING -> ";
			if ( lit->second.autoindex == true )
				std::cout << "TRUE" << std::endl;
			else
				std::cout << "FALSE" << std::endl;
			std::cout << "HTTP REDIRECTION FOR ERROR CODE -> ";
			std::cout << lit->second.http_redirection.first << " ";
			std::cout << "WILL BE REDIRECTED TO -> " << lit->second.http_redirection.second << std::endl;
			std::cout << "EVERY ACCEPTED METHOD - > " << std::endl;
			if ( lit->second.GET == true )
				std::cout << "GET" << std::endl;
			if ( lit->second.POST == true )
				std::cout << "POST" << std::endl;
			if ( lit->second.DELETE == true )
				std::cout << "DELETE" << std::endl;
			std::cout << "UPLOADED FILES WILL BE STORED IN -> ";
			std::cout << lit->second.upload_path << std::endl;

			std::cout << std::endl;
		}
		i++;
		std::cout << std::endl;
	}
	std::cout << WHITE << std::endl;
}

/////////////////////////////////////////////////////
///                                               ///
///          			UTILS             		  ///
///                                               ///
/////////////////////////////////////////////////////



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


std::pair<std::string, std::vector<std::string> > parseConfig::insert_port( std::string raw_address ) //! Cannot pass by reference
{
	std::string address;
	std::string port;
	std::string ip_address;

	address.assign(trim_data(raw_address, "listen"));

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

	std::vector<std::string> ports(1, port);

	std::pair<std::string, std::vector<std::string> > ret(ip_address, ports);

	return ret;
}

std::vector<std::string> parseConfig::insert_server_names( std::string & raw_server_name )
{
	raw_server_name = trim_data(raw_server_name, "server_name");

	std::list<std::string> 		tmp = ft_split(raw_server_name, " ");
	std::vector<std::string> 	server_names;

	for ( std::list<std::string>::iterator it = tmp.begin() ; it != tmp.end() ; it++ )
		if (!it->empty())
			server_names.push_back(*it);

	return server_names;
}

//! Cannot pass by reference in this function (because of its use)
std::string parseConfig::trim_data( std::string & raw_data, const std::string & data_name ) //! Trim at the begin and at the end every isspace, and transform every isspace that is not in the beginning or at the end, to a space ' ' for parsing purpose
{
 	if ( raw_data.find(data_name) != std::string::npos )
		raw_data.erase(raw_data.find(data_name), data_name.size());
	
	if ( raw_data.find(";") != std::string::npos )
		raw_data.erase(raw_data.find(";"));
	
	std::string::iterator it = raw_data.begin();
	while ( it != raw_data.end() && isspace(*it) )
		it++;
	
	std::string::reverse_iterator rit = raw_data.rbegin();
	while ( rit != raw_data.rend() && isspace(*rit) )
		rit++;

	std::string::iterator trim_end = rit.base();

	std::string trimmed_raw_data(it, trim_end);

	while ( trimmed_raw_data.find("\t") != std::string::npos )
		trimmed_raw_data[trimmed_raw_data.find("\t")] = ' ';

	return trimmed_raw_data;
}

int	parseConfig::insert_body_max_size( std::string & raw_data )
{
	std::string	data = trim_data(raw_data, "client_max_body_size");
	std::string	first_value;

	for ( std::string::size_type i = 0 ; isdigit(data[i]) ; i++ )
		first_value.append(1, data[i]);
	
	if ( first_value.empty() )
		return -1;

	return std::atoi(first_value.c_str());
}

std::pair<std::vector<int>, std::string> parseConfig::insert_error_page( std::string & raw_error_page )
{
	std::vector<int> 		errors;
	std::string 	 		error_path;
	std::string		 		error_code;
	std::string::size_type 	i = 0;

	std::string error_page = trim_data(raw_error_page, "error_page");

	for ( ; i < error_page.size() ; i++ )
	{
		if ( error_page[i] == '/' )
			break ;
		
		for ( ; isdigit(error_page[i]) ; i++ )
			error_code.append(1, error_page[i]);

		if ( !error_code.empty() )
		{
			errors.push_back(std::atoi(error_code.c_str()));
			error_code.clear();
		}
	}
	error_path.append(error_page, i, std::string::npos);

	std::pair<std::vector<int>, std::string> ret(errors, error_path);

	return ret;
}

int parseConfig::count_server( std::list<std::string>::iterator it, std::list<std::string>::iterator ite )
{
	int n = 0;
	int closed = 0;

	for ( ; it != ite ; it++ )
	{
		if ( it->find("server") != std::string::npos)
		{
			while ( it != ite )
			{
				if ( (it->find("}") != std::string::npos) && closed == 1 )
				{
					n++;
					break ;
				}
				if ( it->find("{") != std::string::npos )
					closed = 1;
				it++;
			}
		}
		if ( it == ite )
			break ;
	}

	return n;
}

bool	parseConfig::check_closure( std::string & line )
{
	if ( (line.find(";") == std::string::npos) && (exact_match(line, "location") == false) && _closed == 1 && _inside == 1)
	{
		_actual_error = "MISSING ; OR UNKNOWN COMMAND : ";
		_actual_error.append(line);
		_state = false;
		return false ;
	}

	if ( line.find("{") != std::string::npos )
	{
		_closed = 1;
		_inside = 1;
	}

	return true;
}

std::string	 parseConfig::get_location_path( std::string & line )
{
	std::string new_line;

	line = trim_data(line, "location");

	for ( std::string::size_type i = 0; !isspace(line[i]) && line[i] ; i++ )
		new_line.append(1, line[i]);
	
	if ( new_line.empty() )
		return "";

	return new_line;
}

//! IT MUST NOT BE BY REFERENCE, or it will increment the local iterator
size_t	parseConfig::check_location( std::list<std::string>::iterator it, std::list<std::string>::iterator ite )
{
	int open = 0;
	int	closed = 0;

	for ( ; it != ite ; it++ )
	{
		if ( it->find("{") != std::string::npos )
			open++;
		
		if ( (it->find("}") != std::string::npos) )
		{
			it++;
			if ( it->empty() || ( it->at(it->length() - 1) != ';' && it->find("location") == std::string::npos && *it != "}") )
			{
				_actual_error = "MISSING A CLOSURE BRACE IN A LOCATION ";
				_state = false;	
				return std::string::npos;
			}
			closed++;
			break ;
		}
	}

	if ( open == 1 && closed == 1 )
		return 0;

	_actual_error = "SOMETHING IS WRONG IN A LOCATION BRACES";
	_state = false;	

	return std::string::npos;
}

std::string	parseConfig::L_insert_root( std::string & line )
{
	std::string new_line;

	line = trim_data(line, "root");

	for ( std::string::size_type i = 0; !isspace(line[i]) && line[i] ; i++ )
		new_line.append(1, line[i]);
	
	return new_line;
}

bool	parseConfig::exact_match( std::string & raw_str, const std::string & keyword )
{
	std::string 			str = trim_data(raw_str, "");
	std::string::size_type 	pos = str.find(keyword);

	if ( pos == std::string::npos )
		return false;

	if ( pos != 0 || !isspace(str[keyword.size()]))
		return false;
	
	return true;
}

std::pair<int, std::string> parseConfig::insert_http_redirection( std::string & raw_line )
{
	raw_line = trim_data(raw_line, "return");

	std::pair<int, std::string> ret;
	std::list<std::string>		http_redir = ft_split(raw_line, " ");

	std::list<std::string>::iterator it = http_redir.begin();
	std::list<std::string>::iterator ite = http_redir.end();

	for ( ; it != ite ; it++ )
		if (it->empty())
			it = remove_empty_line(http_redir, it);
	
	if ( http_redir.size() > 1 )
		ret = std::make_pair(std::atoi(http_redir.front().c_str()), http_redir.back());
	else
		ret = std::make_pair(-1, "");

	return ret;
}

void	parseConfig::insert_method( std::string & raw_method, const std::string & location )
{
	raw_method = trim_data(raw_method, "method_accept");

	std::list<std::string> 				methods = ft_split(raw_method, " ");
	std::list<std::string>::iterator 	it = methods.begin();
	std::list<std::string>::iterator 	ite = methods.end();

	for ( ; it != ite ; it++ )
		if (it->empty())
			it = remove_empty_line(methods, it);
	
	it = methods.begin();

	for ( ; it != ite ; it++ )
	{
		if ( *it == "GET" )
			_config.locations[location].GET = true;
		if ( *it == "POST" )
			_config.locations[location].POST = true;
		if ( *it == "DELETE" )
			_config.locations[location].DELETE = true;
	}
	
}

std::string parseConfig::insert_upload_path( std::string & line )
{
	std::string new_line;

	line = trim_data(line, "set_upload");

	for ( std::string::size_type i = 0; !isspace(line[i]) && line[i] ; i++ )
		new_line.append(1, line[i]);
	
	return new_line;
}

//! Must be a copy, not a value (Can change it later)
std::list<std::string>::iterator	parseConfig::parse_location( std::list<std::string>::iterator it, std::list<std::string>::iterator ite )
{
	std::string 	path;
	int 			closed = 0;
	int	 			inside = 0;

	path = get_location_path(*it);
	if ( path.empty() )
	{
		_actual_error = "MISSING LOCATION PATH : ";
		_actual_error.append(*it);
		_state = false;
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
			_actual_error = "MISSING ; : ";
			_actual_error.append(*it);
			_state = false;
			return _file.end();
		}
		if ( it->find("{") != std::string::npos )
		{
			inside = 1;
			closed = 1;
		}

		if ( exact_match(*it, "root") == true )
			_config.locations[path].root = L_insert_root(*it);
		
		else if ( exact_match(*it, "autoindex") == true )
			_config.locations[path].autoindex = insert_index(*it);
		
		else if ( exact_match(*it, "return") == true )
			_config.locations[path].http_redirection = insert_http_redirection(*it);
		
		else if ( exact_match(*it, "method_accept") == true )
			insert_method(*it, path);
		else if ( exact_match(*it, "set_upload") == true )
			_config.locations[path].upload_path = insert_upload_path(*it);
	}

	return it;
}

bool	parseConfig::insert_index( std::string & raw_index )
{
	std::string index = trim_data(raw_index, "autoindex");
	std::string raw_state;
	bool 		state = false;

	for ( std::string::size_type i = 0; !isspace(index[i]) && index[i] ; i++ )
		raw_state.append(1, index[i]);
	
	if ( raw_state == "on" )
		state = true;
	else
		state = false;

	return state;
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
			_config.body_max_size = insert_body_max_size(line);
			if ( _config.body_max_size == -1 )
			{
				_actual_error = "ERROR AT : ";
				_actual_error.append(line);
				_state = false;
				return false;
			}
		}

		else if ( exact_match(line, "error_page") == true )
			_config.errors.push_back(insert_error_page(line));

		else if ( exact_match(line, "autoindex") == true )
			_config.locations["/"].autoindex = insert_index(line);
		
		else if ( exact_match(line, "return") == true )
			_config.locations["/"].http_redirection = insert_http_redirection(line);
		
		else if ( exact_match(line, "method_accept") == true )
			insert_method(line, "/");
		
		else if ( exact_match(line, "set_upload") == true )
			_config.locations["/"].upload_path = insert_upload_path(line);
	
		else if ( line != *(_file.begin()) && line.find_first_of("{}") == std::string::npos )
		{
			_actual_error = "UNKOWN COMMAND : ";
			_actual_error.append(line);
			_state = false;
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
					{
						_actual_error = "INVALID CLOSURE ";
						_state = false;
						return ;
					}
					break ;
				}

				if ( check_closure(*it) == false )
					return ;

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
			if ( _config.listening.empty() )
				_config.listening.insert(insert_port("localhost:8080"));
		}
		_configs.push_back(_config);
		_config.clear();
		_inside = 0;
		_closed = 0;
		
		if ( it == ite )
			break ;
	}
	print_all_informations();
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