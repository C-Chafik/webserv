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

std::pair<std::string, std::string> parseConfig::insert_port( std::string raw_address )
{
	std::string address;
	std::string port;
	std::string ip_address;

	if ( raw_address.find("listen") != std::string::npos )
		raw_address.erase(raw_address.find("listen"), 7);
	while ( raw_address.find_first_of(" \t") != std::string::npos )
	{
		raw_address.erase(raw_address.find_first_of(" \t"), 1);
	}

	address.assign(raw_address);

	if ( address.find(";") != std::string::npos )
		address.erase(address.find(";"));
	
	if ( address.find(":") != std::string::npos )
	{
		ip_address = address.substr(0, address.find(":"));
		port = address.substr(address.find(":"), std::string::npos );
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
	// std::cout << "PORT = " << port << std::endl;
	// std::cout << "ip_address = " << ip_address << std::endl;
	return std::pair<std::string, std::string>(ip_address, port);
}

std::vector<std::string> parseConfig::insert_server_names( std::string raw_server_name )
{
	//! faire une fonction qui automatise cette partie la du parsing.... (psk c'est bcp trop moche la)

	if ( raw_server_name.find("server_name") != std::string::npos )
		raw_server_name.erase(raw_server_name.find("server_name"), 11);
	if ( raw_server_name.find(";") != std::string::npos )
		raw_server_name.erase(raw_server_name.find(";"));
	for ( std::string::size_type i = 0 ; (raw_server_name[i] == '\t' || raw_server_name[i] == ' ') ; i++ )
	{
		raw_server_name.erase(i, 1);
	}
	for ( std::string::size_type i = raw_server_name.length() - 1 ; (raw_server_name[i] == '\t' || raw_server_name[i] == ' ') ; i-- )
	{
		raw_server_name.erase(i, 1);
	}
	while ( raw_server_name.find("\t") != std::string::npos )
		raw_server_name[raw_server_name.find("\t")] = ' ';
	std::list<std::string> tmp = ft_split(raw_server_name, " ");
	std::vector<std::string> server_names;

	for ( std::list<std::string>::iterator it = tmp.begin() ; it != tmp.end() ; it++ )
	{
		if (!it->empty())
			server_names.push_back(*it);
	}
	return server_names;
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
			_inside = 1;
			while ( it != ite )
			{
				if ( it->find("{") != std::string::npos )
					_closed = 1;
				if ( (it->find("}") != std::string::npos ) && _closed == 1 )
				{
					std::cout << "break ;" << std::endl;
					break ;
				}
				if ( it->find("listen") != std::string::npos )
					_config.listening.insert(insert_port(*it));
				if ( it->find("server_name") != std::string::npos )
					_config.server_names = insert_server_names(*it);
				it++;
			}
		}
		if ( it == ite )
			break ;
	}
	if ( _config.listening.empty() )
		_config.listening.insert(insert_port("localhost:80"));
	for ( std::vector<std::string>::iterator it = _config.server_names.begin() ; it !=  _config.server_names.end(); it++ )
	{
		std::cout << *it << " ";
	}
}