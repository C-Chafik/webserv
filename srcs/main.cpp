#include "includes.hpp"

// std::vector< std::thread > *global = NULL;

int main(){

	parseConfig parse("conf/default.conf");
	if ( parse.state() == false )
		return parse.exit_on_error();
	
	Server serv;
	std::vector< struct config > confs = parse.get_config();

	std::vector< std::thread > threads;

	serv.conf = confs[0];
	serv.run();

	return EXIT_SUCCESS;
}
