std::list<std::string> ft_split( const std::string & header, const std::string & charset );
std::list<std::string> ft_split_no_r( const std::string & header, const std::string & charset );
std::string trim_data( std::string & raw_data, const std::string & data_name );
std::string trim_data_semicolon( std::string & raw_data, const std::string & data_name );
std::string 	insertion_semicolon( std::string & line, const std::string & name );
std::string	 insertion( std::string & line, const std::string & name );