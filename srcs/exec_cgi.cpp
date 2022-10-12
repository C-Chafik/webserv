#include "includes.hpp"

char **Server::create_arg(std::string php_arg){
	char **rtn = new char*[3];
	std::string ex = "php-cgi";


	rtn[0] = new char[ex.size()+1];
	rtn[0] = strcpy(rtn[0], (const char*)ex.c_str());
	rtn[1] = new char[php_arg.size()+1];
	rtn[1] = strcpy(rtn[1], (const char*)php_arg.c_str());
	rtn[2] = NULL;

	return rtn;
}

std::string Server::cgi_exec(struct header & header, id_server_type server_id, std::string php_arg, std::string method, char **env){
	(void)method;
	(void)header;
	(void)server_id;
	(void)php_arg;
	(void)env;
	FILE* fileIn = tmpfile();
	FILE* fileOut = tmpfile();
	int fdFileIn = fileno(fileIn);
	int fdFileOut = fileno(fileOut);
	int saveIn = dup(STDIN_FILENO);
	int saveOut = dup(STDOUT_FILENO);
	std::string output;

	if (method == "POST"){
		std::string body = fileToString("/tmp/cgi_post.log");
		write(fdFileIn, body.c_str(), body.size());
		lseek(fdFileIn, 0, SEEK_SET);
		// char test[body.size()];
		// read(fdFileIn, test, body.size());
		// std::clog << "["<< test << "]"<< std::endl;
	}

	int pid = fork();

	if (pid < 0){
		std::cerr << "Error when forking for php-cgi" << std::endl;
		return ("Status: 500\n\n");
	}

	if (pid == 0){
		char ** arg = create_arg(php_arg);

		dup2(fdFileIn, STDIN_FILENO);
		dup2(fdFileOut, STDOUT_FILENO);

		execve(confs[server_id].cgi_path.c_str(), arg, env);

		for (size_t i = 0; arg[i]; i++){
			// std::clog << "=" << arg[i] << "=" << std::endl;
			delete[] arg[i];
		}
		delete[] arg;
		write(STDOUT_FILENO, "Status: 500\n\n", 13);
		exit (1);
	}
	else{
		wait(NULL);
		lseek(fdFileOut, 0, SEEK_SET);
		
		ssize_t rtn = 1;
		char buff[100];

		while (rtn){
			memset(buff, 0, 100);
			rtn = read(fdFileOut, buff, 99);
			// std::clog << "-> " << buff << std::endl;
			output.append(buff);
		}
	}
	
	dup2(saveIn, STDIN_FILENO);
	dup2(saveOut, STDOUT_FILENO);
	fclose(fileIn);
	fclose(fileOut);
	close(fdFileIn);
	close(fdFileOut);
	close(saveIn);
	close(saveOut);

	// std::clog << "\""<< output<< "\"" << std::endl;
	return output;
}