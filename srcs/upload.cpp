#include "../includes/server.hpp"


//! The upload need to be merged with the configuration file
//! need to undurstand it further
//! This function must check the max body size !!!
void	Server::treat_POST_request( struct header & head, struct body & bod )
{
	std::fstream 	file;
	std::string		path;   //! need to use the conf file to get that path

	if ( head.content_type == "multipart/form-data" )
	{
		std::string filename;
		std::string line;
		std::string content;

		//! setting up the loop
		content = bod.content.substr(bod.content.find("\n") + 1);
		line = content.substr(0, content.find("\n"));
		content = content.substr(content.find("\n") + 1); //! way to iterate

		while ( line != head.boundary + "--\r" && !content.empty() )
		{
			if ( line.substr(0, 20) == "Content-Disposition:" )
			{
				std::list<std::string> name = ft_split_no_r(line, " \r");
				filename = name.back().substr(10);
				filename = filename.substr(0, filename.find("\""));
			}
			else if ( line == "\r" )
			{
				if (filename.empty())
					filename = "TEMPORARY";
				path = path + filename;
				file.open(path.c_str(), std::ios::out);
				if ( !file.is_open() )
				{
					std::cout << "ERROR CREATING THE UPLOAD FILE " << std::endl;
					return ;
				}
				line = content.substr(0, content.find("\n"));
				content = content.substr(content.find("\n") + 1);
				while ( !content.empty() )
				{
					std::string tmp = line;

					line = content.substr(0, content.find("\n"));
					content = content.substr(content.find("\n") + 1);
					if ( line == head.boundary + "\r" || line == head.boundary + "--\r" || content.empty() )
					{
						//? removing last \r character  
						if ( tmp.length() >= 1 )
							tmp.erase(tmp.end() - 1);
						
						file << tmp;
						break ;
					}
					file << tmp;
					file << '\n';
				}
				file.close();
			}
			if ( line != head.boundary + "--\r" && !content.empty() )
			{
				line = content.substr(0, content.find("\n"));
				content = content.substr(content.find("\n") + 1);
			}
		}
		if ( line != head.boundary + "--\r" && !content.empty() )
        {
			std::cout << "error" << std::endl;
        }
	}
	else
	{
		std::string filename;
		
		if (filename.empty())
			filename = "TEMPORARY";
		path = path + filename;
		file.open(path.c_str(), std::ios::out);
		if ( !file.is_open() )
		{
			std::cout << "ERROR CREATING THE UPLOAD FILE " << std::endl;
			return ;
		}
		file << bod.content;
		file.close();
	}
}   