#include "../includes/parseConfig.hpp"

/////////////////////////////////////////////////////
///                                               ///
///          CONSTRUCTOR - DESTRUCTOR             ///
///                                               ///
/////////////////////////////////////////////////////

parseConfig::parseConfig( void )
{

}

parseConfig::parseConfig( std::string path ) : _file_path(path)
{
	_closed = 0;
	_inside = 0;
	_state = false;
	if ( fill_file() == false )
		return ;
	parse_file();
	if (_state == false)
		return ;
}

parseConfig::~parseConfig( void )
{

}

/////////////////////////////////////////////////////
///                                               ///
///          CONSTRUCTOR - DESTRUCTOR             ///
///                                               ///
/////////////////////////////////////////////////////

//! ft_split

std::list<std::string> parseConfig::ft_split(std::string header, std::string charset )
{
	size_t start = 0, end, charset_len = charset.length();
	std::string tmp;
	std::list<std::string> ret;
	while ((end = header.find(charset, start)) != std::string::npos)
	{
		tmp = header.substr(start, 	end - start);
		start = end + charset_len;
		ret.push_back(tmp);
	}
	ret.push_back(header.substr(start));
	return ret;		
}


//! ft_split

//* Getter

bool	parseConfig::state ( void )
{
	return _state;
}

//* Getter


int parseConfig::exit_on_error ( void )
{
	std::cout << RED << _actual_error << std::endl;
	return 1;
}

void	parseConfig::remove_tab( std::string & str )
{
	for ( std::string::size_type i = 0 ; str[i] == '\t' ; i++ )
	{
		str.erase(i, 1);
	}
}

std::list<std::string>::iterator parseConfig::remove_empty_line(  std::list<std::string>::iterator it )
{
	if (it->empty())
		_file.erase(it);
	std::list<std::string>::iterator new_it = _file.begin();
	return new_it;
}


bool parseConfig::fill_file( void )
{
	std::ifstream file;
	std::string buffer;
	std::string	fileSTR;

	file.open(_file_path);
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
	{
		remove_tab(*it);
	}
	for ( std::list<std::string>::iterator it = _file.begin() ; it != _file.end() ; it++ )
	{
		if (it->empty())
		{
			it = remove_empty_line(it);
		}
	}
	// for ( std::list<std::string>::iterator it = _file.begin() ; it != _file.end() ; it++ )
	// {
	// 	std::cout << "[" << *it << "]" << std::endl;
	// }
	return true;
}

void	parseConfig::print_all_informations( void )
{
	std::cout << MAGENTA << "PRINTING ALL GATHERED INFORMATIONS " << std::endl << std::endl;
	std::map<std::string, std::vector<std::string> >::iterator mit = _config.listening.begin();
	std::map<std::string, std::vector<std::string> >::iterator mite = _config.listening.end();

	std::vector<std::string>::iterator vit = _config.server_names.begin();
	std::vector<std::string>::iterator vite = _config.server_names.end();

	std::vector< std::pair<std::vector<int>, std::string> >::iterator eit = _config.errors.begin();
	std::vector< std::pair<std::vector<int>, std::string> >::iterator eite = _config.errors.end();


	std::cout << "ALL LISTEN PORT AND IP " << std::endl;
	for ( ; mit != mite ; mit++ )
	{
		std::cout << mit->first << " ";
		for ( std::vector<std::string>::iterator it = mit->second.begin() ; it != mit->second.end() ; it++ )
		{
			std::cout << *it <<" : ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout << "ALL SERVER NAMES " << std::endl;
	for ( ; vit != vite ; vit++ )
	{
		std::cout << *vit << " ";
	}
	std::cout << std::endl;
	std::cout << "CLIENT_MAX_BODY_SIZE " << std::endl;
	std::cout << _config.body_max_size << std::endl;
	std::cout << "ALL ERRORS PAGE" << std::endl;
	for ( ; eit != eite ; eit++ )
	{
		std::cout << "ERROR PATH : " << eit->second << " FOR ERROR CODE -> ";
		for ( std::vector<int>::iterator it = eit->first.begin() ; it != eit->first.end() ; it++ )
		{
			std::cout << *it <<": ";
		}
		std::cout << std::endl;
	}
	std::cout << WHITE << std::endl;
}

std::pair<std::string, std::vector<std::string> > parseConfig::insert_port( std::string raw_address )
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
		port.assign("80");
	}
	else if ( address == "localhost" )
	{
		port.assign("80");
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

std::vector<std::string> parseConfig::insert_server_names( std::string raw_server_name )
{
	raw_server_name = trim_data(raw_server_name, "server_name");
	std::list<std::string> tmp = ft_split(raw_server_name, " ");
	std::vector<std::string> server_names;

	for ( std::list<std::string>::iterator it = tmp.begin() ; it != tmp.end() ; it++ )
	{
		if (!it->empty())
			server_names.push_back(*it);
	}
	return server_names;
}

std::string parseConfig::trim_data( std::string raw_data, std::string data_name ) //! Trim at the begin and at the end every isspace, and transform every isspace that is not in the beginning or at the end, to a space ' ' for parsing purpose
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

	std::string trimmed_raw_data(it, raw_data.end());

	while (trimmed_raw_data.find("\t") != std::string::npos )
		trimmed_raw_data[trimmed_raw_data.find("\t")] = ' ';
	return trimmed_raw_data;
}

int	parseConfig::insert_body_max_size( std::string raw_data )
{
	std::string data = trim_data(raw_data, "client_max_body_size");
	
	std::string first_value;
	for ( std::string::size_type i = 0 ; isdigit(data[i]) ; i++ )
	{
		first_value.append(1, data[i]);
	}
	if ( first_value.empty() )
		return -1;
	return std::atoi(first_value.c_str());
}

std::pair<std::vector<int>, std::string> parseConfig::insert_error_page( std::string raw_error_page )
{
	std::vector<int> errors;
	std::string 	 error_path;
	std::string		 error_code;
	std::string::size_type i = 0;


	std::string error_page = trim_data(raw_error_page, "error_page");


	for ( ; i < error_page.size() ; i++ )
	{
		if ( error_page[i] == '/' )
			break ;
		for ( ; isdigit(error_page[i]) ; i++ )
		{
			error_code.append(1, error_page[i]);
		}
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
				if ( (it->find("}") != std::string::npos ) && _closed == 1 )
				{
					if ( it->back() != '}' )
					{
						_actual_error = "INVALID CLOSURE ";
						_state = false;
						return ;
					}
					std::cout << "break ;" << std::endl;
					break ;
				}
				if ( (it->find(";") == std::string::npos) && _closed == 1 && _inside == 1)
				{
					_actual_error = "MISSING ; : ";
					_actual_error.append(*it);
					_state = false;
					return ;
				}
				if ( it->find("{") != std::string::npos )
				{
					_closed = 1;
					_inside = 1;
				}
				if ( it->find("listen") != std::string::npos )
				{
					std::pair<std::map<std::string, std::vector<std::string> >::iterator, bool > ret;
					std::pair<std::string, std::vector<std::string> > listen(insert_port(*it));

					ret = _config.listening.insert(listen);
					if ( ret.second == false )
						ret.first->second.push_back(listen.second.front());
				}
				else if ( it->find("server_name") != std::string::npos )
					_config.server_names = insert_server_names(*it);
				else if ( it->find("client_max_body_size") != std::string::npos )
				{
					_config.body_max_size = insert_body_max_size(*it);
					if ( _config.body_max_size == -1 )
					{
						_actual_error = "ERROR AT : ";
						_actual_error.append(*it);
						_state = false;
						return ;
					}
				}
				else if ( it->find("error_page") != std::string::npos )
				{
					_config.errors.push_back(insert_error_page(*it));
				}
				it++;
			}
		}
		_inside = 0;
		_closed = 0;
		if ( it == ite )
			break ;
	}
	if ( _config.listening.empty() )
		_config.listening.insert(insert_port("localhost:80"));
	print_all_informations();
}