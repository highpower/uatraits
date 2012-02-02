#include "acsetup.hpp"

#include <map>
#include <cstdlib>
#include <iostream>
#include <exception>

#include "uatraits/detector.hpp"

int
main(int argc, char *argv[]) {

	if (argc != 2) {
		std::cout << "Usage: check-uatraits <browsers.xml> < <user-agents>" << std::endl;
		return EXIT_SUCCESS;
	}
	try {
		uatraits::detector engine(argv[1]);
	        std::string line;
		while (std::getline(std::cin, line)) {
			if (line.empty()) {
				continue;
			}

			std::cout << line << std::endl;

			typedef uatraits::detector::result_type result_type;
			result_type res;
			engine.detect(line, res);
			for (result_type::const_iterator it = res.begin(), end = res.end(); it != end; ++it) {
				std::cout << " " << it->first << ": " << it->second << std::endl;
			}
		}
		return EXIT_SUCCESS;
	}
	catch (std::exception const &e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
}
