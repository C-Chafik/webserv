#include "includes.hpp"

int main(){

	parseConfig parse("conf/default.conf");//do a dynamic search and go to default location if nothig
	if ( parse.state() == false )
		return parse.exit_on_error();
	
	Server serv;
	std::vector< struct config > confs = parse.get_config();

	serv.conf = confs[0];
	serv.run(confs);

	return EXIT_SUCCESS;
}
