#include "includes.hpp"

int main(){
	std::thread one(server, 9998);
	std::thread two(server, 9999);

	one.join();
	two.join();
	return EXIT_SUCCESS;
}