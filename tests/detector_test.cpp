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

BOOST_AUTO_TEST_CASE(test_mobile_safari) {

	std::string line =
		"Mozilla/5.0 (Linux; U; Android 2.3; ru-ru; Desire_A8181 Build/FRF91) "
		"AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1";

	detector det(getenv("DATAFILE"));
	std::map<std::string, std::string> result = det.detect(line);

	BOOST_CHECK_EQUAL(result.find("BrowserEngine")->second, "WebKit");
	BOOST_CHECK_EQUAL(result.find("BrowserName")->second, "MobileSafari");
	BOOST_CHECK_EQUAL(result.find("OSFamily")->second, "Android");
	BOOST_CHECK_EQUAL(result.find("isMobile")->second, "true");
	BOOST_CHECK_EQUAL(result.find("isTouch")->second, "true");
}

BOOST_AUTO_TEST_SUITE_END();

}} // namespaces
