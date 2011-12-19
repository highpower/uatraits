#include "acsetup.hpp"

#include <boost/test/unit_test.hpp>

#include "uatraits/details/integer.hpp"
#include "uatraits/details/type_traits.hpp"

namespace uatraits { namespace tests {

BOOST_AUTO_TEST_SUITE(integer_test)

BOOST_AUTO_TEST_CASE(test_typedefs) {

	BOOST_CHECK_EQUAL(1, sizeof(details::int8_t));
	BOOST_CHECK_EQUAL(1, sizeof(details::uint8_t));

	BOOST_CHECK_EQUAL(2, sizeof(details::int16_t));
	BOOST_CHECK_EQUAL(2, sizeof(details::uint16_t));

	BOOST_CHECK_EQUAL(4, sizeof(details::int32_t));
	BOOST_CHECK_EQUAL(4, sizeof(details::uint32_t));

	BOOST_CHECK_EQUAL(8, sizeof(details::int64_t));
	BOOST_CHECK_EQUAL(8, sizeof(details::uint64_t));
}

BOOST_AUTO_TEST_CASE(test_make_signed) {
	
	using namespace details;
	
	BOOST_CHECK((is_same<signed char, make_signed<char>::type>::result));
	BOOST_CHECK((is_same<signed char, make_signed<unsigned char>::type>::result));

	BOOST_CHECK((is_same<short, make_signed<short>::type>::result));
	BOOST_CHECK((is_same<short, make_signed<unsigned short>::type>::result));

	BOOST_CHECK((is_same<int, make_signed<int>::type>::result));
	BOOST_CHECK((is_same<int, make_signed<unsigned int>::type>::result));

	BOOST_CHECK((is_same<long, make_signed<long>::type>::result));
	BOOST_CHECK((is_same<long, make_signed<unsigned long>::type>::result));

	BOOST_CHECK((is_same<long long, make_signed<long long>::type>::result));
	BOOST_CHECK((is_same<long long, make_signed<unsigned long long>::type>::result));
}

BOOST_AUTO_TEST_CASE(test_make_unsigned) {

	using namespace details;
	
	BOOST_CHECK((is_same<unsigned char, make_unsigned<char>::type>::result));
	BOOST_CHECK((is_same<unsigned char, make_unsigned<unsigned char>::type>::result));

	BOOST_CHECK((is_same<unsigned short, make_unsigned<short>::type>::result));
	BOOST_CHECK((is_same<unsigned short, make_unsigned<unsigned short>::type>::result));

	BOOST_CHECK((is_same<unsigned int, make_unsigned<int>::type>::result));
	BOOST_CHECK((is_same<unsigned int, make_unsigned<unsigned int>::type>::result));

	BOOST_CHECK((is_same<unsigned long, make_unsigned<long>::type>::result));
	BOOST_CHECK((is_same<unsigned long, make_unsigned<unsigned long>::type>::result));

	BOOST_CHECK((is_same<unsigned long long, make_unsigned<long long>::type>::result));
	BOOST_CHECK((is_same<unsigned long long, make_unsigned<unsigned long long>::type>::result));
}

BOOST_AUTO_TEST_SUITE_END();

}} // namespace
