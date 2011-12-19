#include "acsetup.hpp"

#include <string>
#include <boost/test/unit_test.hpp>

#include "mock_template.hpp"
#include "uatraits/details/type_traits.hpp"

namespace uatraits { namespace tests { 

BOOST_AUTO_TEST_SUITE(type_traits_test)

BOOST_AUTO_TEST_CASE(test_is_same) {

	BOOST_CHECK_EQUAL(true, (details::is_same<int, int>::result));
	BOOST_CHECK_EQUAL(true, (details::is_same<int const, int const>::result));
	mock_template<details::is_same<char, char>::result> true_inst;
	(void) true_inst;
	
	BOOST_CHECK_EQUAL(false, (details::is_same<int, double>::result));
	BOOST_CHECK_EQUAL(false, (details::is_same<int, int const>::result));
	mock_template<details::is_same<char, char const>::result> false_inst;
	(void) false_inst;
}

BOOST_AUTO_TEST_CASE(test_is_const) {
	BOOST_CHECK_EQUAL(true, details::is_const<int const>::result);
	BOOST_CHECK_EQUAL(true, details::is_const<std::string const>::result);
	mock_template<details::is_const<char const>::result> true_inst;
	(void) true_inst;
	
	BOOST_CHECK_EQUAL(false, details::is_const<int>::result);
	BOOST_CHECK_EQUAL(false, details::is_const<std::string>::result);
	mock_template<details::is_const<char>::result> false_inst;
	(void) false_inst;
}

BOOST_AUTO_TEST_CASE(test_add_const) {
	BOOST_CHECK_EQUAL(true, (details::is_same<details::add_const<char>::type, char const>::result));
	BOOST_CHECK_EQUAL(true, (details::is_same<details::add_const<char const>::type, char const>::result));
}

BOOST_AUTO_TEST_CASE(test_remove_const) {
	BOOST_CHECK_EQUAL(true, (details::is_same<details::remove_const<char>::type, char>::result));
	BOOST_CHECK_EQUAL(true, (details::is_same<details::remove_const<char const>::type, char>::result));
}

BOOST_AUTO_TEST_SUITE_END();

}} // namespaces
