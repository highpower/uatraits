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
	typedef std::pair<std::string::size_type, std::string::size_type> replace_data;
	std::map<std::size_t, replace_data> data;
	find_replaces("$1abc$2 x $3", data);
	BOOST_CHECK_EQUAL(3, data.size());
	BOOST_CHECK_EQUAL(0, data[1].first);
	BOOST_CHECK_EQUAL(2, data[1].second);
	BOOST_CHECK_EQUAL(5, data[2].first);
	BOOST_CHECK_EQUAL(2, data[2].second);
}

BOOST_AUTO_TEST_SUITE_END();

}} // namespaces