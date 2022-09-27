#include "../includes/includes.hpp"

T_POST::T_POST( void )
{

}

T_POST::~T_POST( void )
{

}

T_POST::T_POST( const std::string & req, const std::string & path ) : _request(req), _content_length(0), _uploading_path(path)
{
	_keep_alive = true;
	std::list<std::string> header = ft_split(_request, "\n");

	if ( get_header_informations(header.begin(), header.end()) == false )
		return ;
	
	_boundary = get_boundary(_content_type);

	if ( get_content(_request) == false )
		return ;

	// std::list<std::string>::iterator 	it = _content.begin();
	// std::list<std::string>::iterator 	ite = _content.end();

	// for ( ; it != ite ; it++)
	// 	std::cout << RED << *it;

	if ( treat_content( _content_type ) == false )
		return ;

	//! return 415 error code if error

	print_all_informations();
}

void	T_POST::print_all_informations( void )
{
	std::cout << GREEN << "CONTENT LENGTH : " << _content_length << WHITE << std::endl;
	std::cout << GREEN << "CONTENT_TYPE : " << _content_type << WHITE << std::endl;
	std::cout << GREEN << "CONTENT : " << _connection << WHITE << std::endl;
	std::cout << GREEN << "REFERER : " << _referer << WHITE << std::endl;
	std::cout << GREEN << "UPLOAD PATH " << _uploading_path << WHITE << std::endl;
}

bool	T_POST::check_keep_connection( void )
{
	return _keep_alive;
}

bool	T_POST::get_content( std::string & req )
{
	size_t erase = 0;

	erase = req.find("\r\n\r\n");
	if ( erase == std::string::npos )
		return false;
	req.erase(0, erase);

	erase = req.find(_boundary);
	if ( erase == std::string::npos )
		return false;
	req.erase(0, erase);

	_content = req;
	return true;
}

std::string T_POST::get_file_name( const std::string & line )
{
	std::string file_name;
	size_t 		erase = 0;

	erase = line.rfind("filename=");

	file_name = line.substr(erase, line.rfind("\"") + 1);
	return file_name;
}

bool	T_POST::upload_file( const std::string & data, const std::string & file_name )
{
	std::cout << "uploading file..." << std::endl;
	std::fstream upload;
	upload.open(("." + _uploading_path + file_name).c_str(), std::fstream::out);
	upload << data;
	upload.close();
	std::cout << "uploaded the file !" << std::endl;
	std::cout << "uploaded the file " << file_name << " at " << _uploading_path << std::endl;
	return true;
}

// bool	T_POST::store_form( std::list<std::string>::iterator it, std::list<std::string>::iterator ite )
// {

// }

bool	T_POST::retrieve_file_content( std::string & content )
{
	return true;
}

bool	T_POST::treat_content( const std::string & encoding )
{
	if ( encoding.find("multipart/form-data") != std::string::npos )
	{
		if ( retrieve_file_content(_content) == false )
			return false;
	}
	// else if ( encoding.find("application/x-www-form-urlencoded") != std::string::npos )
	// {
		// if ( store_form(s_header.begin(), s_header.end()) == false )
		// 	return false;
	// }
	else
		return false;
	return true;
}

std::string  T_POST::get_boundary( std::string & line )
{
	std::string::size_type i = line.length() - 1;
	while ( line[i] != '=' && i != 0 )
		i--;
	if ( line[i] == '=' && i < line.length())
		i++;
	
	return line.substr(i, std::string::npos);
}

bool    T_POST::get_header_informations( std::list<std::string>::iterator it, std::list<std::string>::iterator ite )
{
	bool first = false;

	for ( ; it != ite ; it++ )
	{
		if (it->find("Content-Length:") != std::string::npos)
			_content_length = std::atoi(insertion(*it, "Content-Length:").c_str());
		else if (it->find("Content-Type:") != std::string::npos && first == false )
		{
            _content_type = insertion_semicolon(*it, "Content-Type:");
			first = true;
		}
		else if (it->find("Connection:") != std::string::npos)
		{
            _connection = insertion(*it, "Connection:");
			if ( _connection != "keep-alive")
				_keep_alive = false;
		}
		else if (it->find("Referer:") != std::string::npos)
            _referer = insertion(*it, "Referer:");
	}
	return true;
}

bool    Server::treat_POST_request( const std::string & req, const std::string & path )
{
	T_POST post(req, path);

	return post.check_keep_connection();
}