#include "acsetup.hpp"

#include <boost/test/unit_test.hpp>

#include "mock_shared.hpp"
#include "uatraits/shared_ptr.hpp"

namespace uatraits { namespace tests {

BOOST_AUTO_TEST_SUITE(shared_ptr_test)

BOOST_AUTO_TEST_CASE(test_assign) {
	
	shared_ptr<mock_shared> sptr(new mock_shared());
	shared_ptr<mock_shared_derived> dptr(new mock_shared_derived());

	mock_shared *ps = sptr.get();
	mock_shared_derived *pd = dptr.get();

	BOOST_CHECK_EQUAL(true, mock_shared::is_alive(ps));
	BOOST_CHECK_EQUAL(true, mock_shared::is_alive(pd));
	
	sptr = dptr;
	BOOST_CHECK_EQUAL(true, mock_shared::is_alive(pd));
	BOOST_CHECK_EQUAL(false, mock_shared::is_alive(ps));
}

BOOST_AUTO_TEST_CASE(test_compare) {

	shared_ptr<mock_shared> sptr(new mock_shared());
	shared_ptr<mock_shared_derived> dptr(new mock_shared_derived());
	mock_shared *ps = sptr.get();
	mock_shared_derived *pd = dptr.get();

	BOOST_CHECK_EQUAL(true, sptr == ps);
	BOOST_CHECK_EQUAL(true, sptr != pd);
	BOOST_CHECK_EQUAL(true, dptr != ps);

	BOOST_CHECK_EQUAL(true, mock_shared::is_alive(ps));
	BOOST_CHECK_EQUAL(true, mock_shared::is_alive(pd));
}

BOOST_AUTO_TEST_CASE(test_copy_construct) {

	shared_ptr<mock_shared_derived> sptr(new mock_shared_derived());
	shared_ptr<mock_shared> dptr(sptr);
	BOOST_CHECK_EQUAL(true, sptr == dptr);
	shared_ptr<mock_shared_derived> optr = dptr.cast<mock_shared_derived>();
	BOOST_CHECK_EQUAL(true, sptr == optr);
}

BOOST_AUTO_TEST_CASE(test_default_construct) {

	shared_ptr<mock_shared> sptr;
	BOOST_CHECK(!sptr);
	BOOST_CHECK_EQUAL(false, mock_shared::is_alive(sptr.get()));
	
	sptr.reset(new mock_shared());
	BOOST_CHECK(sptr);
	BOOST_CHECK_EQUAL(true, mock_shared::is_alive(sptr.get()));
}

BOOST_AUTO_TEST_SUITE_END();

}} // namespaces
