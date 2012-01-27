#include "acsetup.hpp"

#include <map>
#include <cstdlib>
#include <iostream>
#include <exception>

#include "uatraits/detector.hpp"

int
main(int argc, char *argv[]) {
	try {
		return EXIT_SUCCESS;
	}
	catch (std::exception const &e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
}
