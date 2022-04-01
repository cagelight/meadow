#include "meadow/clo.hh"

int main(int, char * *) {
	
	meadow::CLO clo;
	
	auto prefix = clo.add_opt_string("prefix");
	
	return 0;
}
