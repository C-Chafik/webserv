#include "../includes/includes.hpp"

std::list<std::string> ft_split( const std::string & header, const std::string & charset )
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

std::string trim_data( std::string & raw_data, const std::string & data_name )
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

std::string trim_data_semicolon( std::string & raw_data, const std::string & data_name )
{
 	if ( raw_data.find(data_name) != std::string::npos )
		raw_data.erase(raw_data.find(data_name), data_name.size());
	
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