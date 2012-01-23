#include "acsetup.hpp"

#include <map>
#include <string>
#include <boost/test/unit_test.hpp>

#include "uatraits/details/regex_utils.hpp"

namespace uatraits { namespace details {

BOOST_AUTO_TEST_SUITE(regex_utils_test)

BOOST_AUTO_TEST_CASE(test_scan_integer) {
	std::string x("abc 234 a56");
	std::size_t result = scan_integer<std::size_t>(x.c_str() + 4, x.c_str() + 9);
	BOOST_CHECK_EQUAL(234, result);
}

BOOST_AUTO_TEST_CASE(test_find_replaces) {

	using namespace details;
	std::vector<regex_data> data;
	find_replaces("$1.$3.$2", data);
	BOOST_CHECK_EQUAL(3, data.size());
	
	BOOST_CHECK_EQUAL(0, data[0].begin);
	BOOST_CHECK_EQUAL(2, data[0].end);
	BOOST_CHECK_EQUAL(1, data[0].index);
	
	BOOST_CHECK_EQUAL(3, data[1].begin);
	BOOST_CHECK_EQUAL(5, data[1].end);
	BOOST_CHECK_EQUAL(3, data[1].index);

	BOOST_CHECK_EQUAL(6, data[2].begin);
	BOOST_CHECK_EQUAL(8, data[2].end);
	BOOST_CHECK_EQUAL(2, data[2].index);
}

BOOST_AUTO_TEST_SUITE_END();

}} // namespaces
