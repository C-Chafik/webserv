#include "../includes/server.hpp"

void	Server::treat_POST_request( struct header & head, struct body & bod, const std::string & file, id_server_type server_id )
{(void)bod;
	std::ifstream 	tmp(file.c_str(), std::ifstream::binary ); //? We first open the raw_data file
	std::fstream 	new_file;
	std::string		path = confs[server_id].locations[head.path].upload_path;   //! need to use the conf file to get that path

	if ( head.content_type == "multipart/form-data" )
	{
		std::string filename;
		std::string line;
		std::string content;

		while ( content.find("\r\n\r\n") == std::string::npos ) //? We skip the first header part, because we just want the body
		{
			std::getline(tmp, line);
			content += line;
			content += '\n';
		}

		while ( std::getline(tmp, line) || line == head.boundary + "--\r" ) //? We iterate trough the file with a gnl until we find the last boundary 
		{
			if ( line.substr(0, 20) == "Content-Disposition:" )
			{
				std::list<std::string> name = ft_split_no_r(line, " \r");
				filename = name.back().substr(10);
				filename = filename.substr(0, filename.find("\""));
			}
			else if ( line.substr(0, 13) == "Content-Type:" )
				continue ;
			else if ( line == "\r" ) //? between boundary you first find informations like content disposition or content type, those informations are separated by a carriage return from the binary file
			{
				std::string file_path = "." + path + filename;
				std::cout << "FINAL UPLOAD PATH IS : " << file_path << std::endl;
				bod.body_path = file_path;
				new_file.open(file_path.c_str(), std::ios::out);
				if ( !new_file.is_open() )
				{
					std::cout << "ERROR CREATING THE UPLOAD FILE " << std::endl;
					return ;
				}
				std::string tmp_line;
				std::string offset;
				std::getline(tmp, tmp_line); 
				while ( std::getline(tmp, offset) )
				{
					if ( offset == head.boundary + "\r" || offset == head.boundary + "--\r" )
					{
						if ( tmp_line.size() >= 1 )
							tmp_line.erase(tmp_line.end() - 1);
						new_file << tmp_line;
						break ;
					}

					new_file << tmp_line;
					new_file << '\n';
					tmp_line = offset;
				}
				new_file.close();
				tmp_line.clear();
			}
			if ( line == head.boundary + "--\r" || line.empty() )
				break ;
		}
	}
	tmp.close();
	remove(file.c_str());
	std::cout <<  strerror(errno) << std::endl;
}   