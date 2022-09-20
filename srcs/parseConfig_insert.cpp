#include "../includes/parseConfig.hpp"

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
		if ( !isdigit(error_page[i]) && !isspace(error_page[i]) )
		{
			while (!isspace(error_page[i]))
				i--;
			if (isspace(error_page[i]))
				i++;
			break ;
		}
		
		for ( ; isdigit(error_page[i]) ; i++ )
			error_code.append(1, error_page[i]);

		if ( !error_code.empty() && (isspace(error_page[i]) || !error_page[i]) )
		{
			errors.push_back(std::atoi(error_code.c_str()));
			error_code.clear();
		}
	}
	if ( i <= error_page.size() )
		error_path.append(error_page, i, std::string::npos);

	std::pair<std::vector<int>, std::string> ret(errors, error_path);

	return ret;
}

std::string	parseConfig::insert_root( std::string & line )
{
	std::string new_line;

	line = trim_data(line, "root");

	for ( std::string::size_type i = 0; !isspace(line[i]) && line[i] ; i++ )
		new_line.append(1, line[i]);
	
	return new_line;
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