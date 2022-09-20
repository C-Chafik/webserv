#include "includes.hpp"

int main(){

	parseConfig parse("conf/default.conf");//do a dynamic search and go to default location if nothig
	if ( parse.state() == false )
		return parse.exit_on_error();
	
	Server serv;

	serv.run( parse.get_config() );

	return EXIT_SUCCESS;
}
