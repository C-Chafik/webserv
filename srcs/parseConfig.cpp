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
	if ( fill_file() == false )
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

// void	parseConfig::remove_tab( std::string & str )
// {
// 	for ( size_t i = 0 ; i < str.size() ; i++ )
// 	{
// 		if ( str.find('\t') )
// 			str.erase(str.find('\t'));
// 	}
// }

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

	// for ( std::list<std::string>::iterator it = _file.begin() ; it != _file.end() ; it++ )
	// {
	// 	remove_tab(*it);
	// }

	for ( std::list<std::string>::iterator it = _file.begin() ; it != _file.end() ; it++ )
	{
		std::cout << *it;
	}
	return true;
}