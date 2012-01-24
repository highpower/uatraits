#include "acsetup.hpp"

#include <list>
#include <vector>
#include <string>
#include <sstream>

#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

#include "test_utils.hpp"
#include "uatraits/details/range.hpp"
#include "uatraits/details/range_utils.hpp"

namespace uatraits { namespace tests {

BOOST_AUTO_TEST_SUITE(range_test)

BOOST_AUTO_TEST_CASE_TEMPLATE(test_output, Sequence, test_param_list) {
	std::stringstream stream;
	Sequence seq = as<Sequence>("test string");
	details::range<typename Sequence::const_iterator> range(seq.begin(), seq.end());
	stream << range;
	BOOST_CHECK_EQUAL(make_string(seq), stream.str());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_reversed, Sequence, test_param_list) {

	Sequence value = as<Sequence>("123 456");
	details::range<typename Sequence::iterator> range(value.begin(), value.end());
	
	BOOST_CHECK_EQUAL(std::string("654 321"), std::string(range.rbegin(), range.rend()));
	BOOST_CHECK_EQUAL(std::string(value.rbegin(), value.rend()), std::string(range.rbegin(), range.rend()));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_construct, Sequence, test_param_list) {

	details::range<typename Sequence::const_iterator> range;
	BOOST_CHECK(range.empty());
	BOOST_CHECK_EQUAL(0, range.size());

	Sequence seq = as<Sequence>("12345");
	details::range<typename Sequence::const_iterator> other = details::make_range(seq);
	BOOST_CHECK(!other.empty());
	BOOST_CHECK_EQUAL(5, other.size());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_comparision, Sequence, test_param_list) {

	Sequence seq = as<Sequence>("test string");
	details::range<char const*> pattern = details::make_range("test string");
	details::range<typename Sequence::const_iterator> empty, range(seq.begin(), seq.end());

	BOOST_CHECK(empty < pattern);
	BOOST_CHECK(range == pattern);
	BOOST_CHECK(range <= pattern);
	BOOST_CHECK(range >= pattern);
	
	BOOST_CHECK_EQUAL(false, range != pattern);
	BOOST_CHECK_EQUAL(false, empty == pattern);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_reversed_comparision, Sequence, test_param_list) {

	Sequence seq = as<Sequence>("test string");
	details::range<char const*> source = details::make_range("test string");
	details::range<typename Sequence::const_reverse_iterator> range(seq.rbegin(), seq.rend());
	details::range<details::range<char const*>::const_reverse_iterator> empty, pattern(source.rbegin(), source.rend());
	
	BOOST_CHECK(empty < pattern);
	BOOST_CHECK(range == pattern);
	BOOST_CHECK(range <= pattern);
	BOOST_CHECK(range >= pattern);

	BOOST_CHECK_EQUAL(false, range != pattern);
	BOOST_CHECK_EQUAL(false, empty == pattern);
}

BOOST_AUTO_TEST_SUITE_END();

}} // namespaces
