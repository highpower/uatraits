#include "acsetup.hpp"

#include <string>
#include <fstream>
#include <boost/test/unit_test.hpp>

#include "uatraits/detector.hpp"

namespace uatraits { namespace tests {

BOOST_AUTO_TEST_SUITE(detector_test)

BOOST_AUTO_TEST_CASE(test_explorer) {

	std::ifstream file("msie.txt");
	file.exceptions(std::ios::badbit | std::ios::failbit);
	detector det(getenv("DATAFILE"));
	file.exceptions(std::ios::badbit);
	
	std::string line;
	while (std::getline(file, line)) {
		std::map<std::string, std::string> result = det.detect(line);
	}
}

BOOST_AUTO_TEST_SUITE_END();

}} // namespaces
