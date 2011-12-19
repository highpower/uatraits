#include "acsetup.hpp"

#include <complex>
#include <typeinfo>
#include <boost/test/unit_test.hpp>

#include "uatraits/details/type_list.hpp"
#include "uatraits/details/type_traits.hpp"

namespace uatraits { namespace tests {

BOOST_AUTO_TEST_SUITE(test_type_list)

BOOST_AUTO_TEST_CASE(test_basics) {

	using namespace details;

	typedef UATRAITS_MAKE_TYPE_LIST2(int, double) test_list;
	typedef type_list<int, type_list<double, null_type>::type> target_list;

	BOOST_CHECK((is_same<test_list, target_list>::result));
}

BOOST_AUTO_TEST_CASE(test_nth_item) {
	
	using namespace details;

	typedef UATRAITS_MAKE_TYPE_LIST9(char, short, int, long, long long, float, double, std::complex<float>, std::complex<double>) test_list;

	BOOST_CHECK((is_same<char, type_list_nth_item<test_list, 0>::type>::result));
	BOOST_CHECK((is_same<short, type_list_nth_item<test_list, 1>::type>::result));
	BOOST_CHECK((is_same<int, type_list_nth_item<test_list, 2>::type>::result));
	BOOST_CHECK((is_same<long, type_list_nth_item<test_list, 3>::type>::result));
	BOOST_CHECK((is_same<long long, type_list_nth_item<test_list, 4>::type>::result));
	BOOST_CHECK((is_same<float, type_list_nth_item<test_list, 5>::type>::result));
	BOOST_CHECK((is_same<double, type_list_nth_item<test_list, 6>::type>::result));
	BOOST_CHECK((is_same<std::complex<float>, type_list_nth_item<test_list, 7>::type>::result));
	BOOST_CHECK((is_same<std::complex<double>, type_list_nth_item<test_list, 8>::type>::result));
}

BOOST_AUTO_TEST_CASE(test_index_of) {

	using namespace details;

	typedef UATRAITS_MAKE_TYPE_LIST9(char, short, int, long, long long, float, double, std::complex<float>, std::complex<double>) test_list;

	BOOST_CHECK_EQUAL(0, (type_list_index_of<test_list, char>::result));
	BOOST_CHECK_EQUAL(1, (type_list_index_of<test_list, short>::result));
	BOOST_CHECK_EQUAL(2, (type_list_index_of<test_list, int>::result));
	BOOST_CHECK_EQUAL(3, (type_list_index_of<test_list, long>::result));
	BOOST_CHECK_EQUAL(4, (type_list_index_of<test_list, long long>::result));
	BOOST_CHECK_EQUAL(5, (type_list_index_of<test_list, float>::result));
	BOOST_CHECK_EQUAL(6, (type_list_index_of<test_list, double>::result));
	BOOST_CHECK_EQUAL(7, (type_list_index_of<test_list, std::complex<float> >::result));
	BOOST_CHECK_EQUAL(8, (type_list_index_of<test_list, std::complex<double> >::result));
}

BOOST_AUTO_TEST_CASE(test_reverse) {

	using namespace details;

	typedef UATRAITS_MAKE_TYPE_LIST9(char, short, int, long, long long, float, double, std::complex<float>, std::complex<double>) test_list;
	typedef UATRAITS_MAKE_TYPE_LIST9(std::complex<double>, std::complex<float>, double, float, long long, long, int, short, char) reversed_list;

	BOOST_CHECK_EQUAL(true, (is_same<reversed_list, type_list_reverse<test_list>::type>::result));
}

BOOST_AUTO_TEST_SUITE_END();

}} // namespaces
