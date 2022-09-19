#include "includes.hpp"

int main(){

	parseConfig parse("conf/default.conf");
	if ( parse.state() == false )
		return parse.exit_on_error();
	
	Server serv;
	std::vector< struct config > confs = parse.get_config();

	serv.conf = confs[0];
	serv.run();

	return EXIT_SUCCESS;
}
