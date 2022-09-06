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
	std::string::size_type i = 0;
	while (i < str.size())
	{
		if ( str.find('\t', i) != std::string::npos )
			str.erase(i, 1);
		i++;
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

	address.assign(raw_address, 7);
	address.erase(address.find(";"));
	// if (address.empty())
	// 	FATAL ERROR
	if ( address.find(":") != std::string::npos )
	{
		ip_address.substr(0, address.find(":"));
		port.substr(address.find(":"), port.length());
	}
	else if ( address.find(".") != std::string::npos )
	{
		ip_address.assign(address);
		port.assign("*");
	}
	else
	{
		port.assign(address);
		ip_address.assign("*");
	}

	return std::pair<std::string, std::string>(ip_address, port);
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
				it++;
			}
		}
		if ( it == ite )
			break ;
	}
}