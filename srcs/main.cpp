#include "includes.hpp"

std::vector< std::thread > *global = NULL;

void endWell(int num){
	std::cerr << std::endl << RED << "WebServ #name# shut down!" << WHITE << std::endl;
	if (global != NULL)
		for (std::vector< std::thread >::iterator it = global->begin(); it != global->end(); it++){
			// it->detach();
			exit(EXIT_SUCCESS);//still reachable and possibly lost
		}
}

int main(){
	std::vector< std::thread > threads;
	global = &threads;

	signal(SIGINT, endWell);

	for (unsigned nb_thread = 0; nb_thread < 5; nb_thread++){
		//parse config file (file = open(env[nb_thread + 1]))
		//give parse struct as parameter to server
		threads.push_back(std::thread(server, 9995 + nb_thread/*tmp*/));//handle connection
	}

	for (std::vector< std::thread >::iterator it = threads.begin(); it != threads.end(); it++)
		it->join();

	return EXIT_SUCCESS;
}

/*
server config = struct_server
location config = vector de pair (std::string location, struct config_loc)
a faire, fonction qui va check la location du fichier et sa config et envoie conportement a adopter
*/