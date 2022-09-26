#include "../includes/includes.hpp"

T_POST::T_POST( void )
{

}

T_POST::~T_POST( void )
{

}

T_POST::T_POST( const std::string & header ) : _header(header)
{
	std::list<std::string> s_header = ft_split(_header, "\n");

	if ( get_header_informations(s_header.begin(), s_header.end()) == false )
		return ;
	
	// if ( get_content(p_POST.content_type, s_header) == false )
	// 	return ;

	//! return 415 error code if error

	print_all_informations();
}

void	T_POST::print_all_informations( void )
{
	std::cout << GREEN << "CONTENT LENGTH : " << p_POST.content_length << WHITE << std::endl;
	std::cout << GREEN << "CONTENT_TYPE : " << p_POST.content_type << WHITE << std::endl;
	std::cout << GREEN << "CONTENT : " << p_POST.connection << WHITE << std::endl;
	std::cout << GREEN << "REFERER : " << p_POST.referer << WHITE << std::endl;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

// bool	T_POST::e_retrieve_content( std::list<std::string>::iterator it, std::list<std::string>::iterator ite )
// {

// }

// bool	T_POST::retrieve_content( std::list<std::string>::iterator it, std::list<std::string>::iterator ite )
// {

// }


bool	T_POST::get_content( const std::string & encoding, std::list<std::string> & s_header )
{(void)s_header;
	if ( encoding.empty() )
		return false;
	else if ( encoding.find("multipart/form-data") != std::string::npos )
	{
		// if ( retrieve_content(s_header.begin(), s_header.end()) == false )
		// 	return false;
	}
	else
	{
		// if ( retrieve_content(s_header.begin(), s_header.end()) == false )
		// 	return false;
	}
	return true;
}

bool    T_POST::get_header_informations( std::list<std::string>::iterator it, std::list<std::string>::iterator ite )
{
	for ( ; it != ite ; it++ )
	{
		if (it->find("Content-Length:") != std::string::npos)
			p_POST.content_length = std::atoi(insertion(*it, "Content-Length:").c_str());
		else if (it->find("Content-Type:") != std::string::npos)
            p_POST.content_type = insertion_semicolon(*it, "Content-Type:");
		else if (it->find("Connection:") != std::string::npos)
            p_POST.connection = insertion(*it, "Connection:");
		else if (it->find("Referer:") != std::string::npos)
            p_POST.referer = insertion(*it, "Referer:");
	}
	return true;
}

bool    Server::treat_POST_request( const std::string & header )
{
	T_POST post(header);

	return true;
}