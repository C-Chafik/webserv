#include "includes.hpp"

std::vector< std::thread > *global = NULL;

void endWell(int num){
	std::cerr << std::endl << RED << "WebServ shut down!" << WHITE << std::endl;
	if (global != NULL)
		for (std::vector< std::thread >::iterator it = global->begin(); it != global->end(); it++){
			// it->detach();
			exit(EXIT_SUCCESS);//still reachable and possibly lost
		}
}

void run_thread(struct config conf){
	Server serv;
	serv.conf = conf;
	serv.run();
}

int main(){

	parseConfig parse("conf/default.conf");
	if ( parse.state() == false )
		return parse.exit_on_error();
	
	Server serv;
	std::vector< struct config > confs = parse.get_config();

	std::vector< std::thread > threads;
	global = &threads;

	signal(SIGINT, endWell);

	unsigned nb_thread = parse.get_config().size();

	for (unsigned i = 0; i < nb_thread; i++){
		//parse config file (file = open(env[nb_thread + 1]))
		//give parse struct as parameter to server
		threads.push_back(std::thread(run_thread, confs[i]));//handle connection
	}

	for (std::vector< std::thread >::iterator it = threads.begin(); it != threads.end(); it++)
		it->join();

	return EXIT_SUCCESS;
}
