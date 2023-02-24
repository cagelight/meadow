#include "tests.hh"

#include <iostream>
#include <string>
#include <map>

static std::map<std::string, void(*)()> const tests {
	{"aeon", test_aeon},
	{"buffer", test_buffer},
	{"brassica", test_brassica},
	{"bspacker", test_bspacker},
	{"noise", test_noise},
	{"oceanus", test_oceanus},
	{"pegleg", test_pegleg},
	{"strop", test_strop},
	{"undae", test_undae}
};

void print_usage() {
	std::cout << "Test Suite Selection:\n";
	for (auto const & kvp : tests) {
		std::cout << "    " << kvp.first << "\n";
	}
	std::cout << std::flush;
	std::exit(1);
}

int main(int argc, char * * argv) {
	if (argc < 2)
		print_usage();
	else {
		auto iter = tests.find(argv[1]);
		if (iter == tests.end()) print_usage();
		else try {
			iter->second();
			tlog << "ALL TESTS SUCCESSFUL";
		} catch (std::exception const & e) {
			tlog << "EXCEPTION OCCURRED DURING TESTING:\n" << e.what();
			std::exit(1);
		}
	}
	return 0;
}
