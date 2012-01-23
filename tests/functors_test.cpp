#include "acsetup.hpp"

#include <list>
#include <vector>
#include <string>

#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

#include "test_utils.hpp"
#include "uatraits/details/range.hpp"
#include "uatraits/details/functors.hpp"

namespace uatraits { namespace tests {

BOOST_AUTO_TEST_SUITE(functors_test)

BOOST_AUTO_TEST_CASE_TEMPLATE(test_ci_less, Sequence, test_param_list) {
	using namespace details;
	ci_less<Sequence> checker;
	BOOST_CHECK(checker(as<Sequence>("Abc"), as<Sequence>("BcD")));
	BOOST_CHECK(!checker(as<Sequence>("Abc"), as<Sequence>("aBc")));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_ci_equal, Sequence, test_param_list) {
	using namespace details;
	ci_equal<Sequence> checker;
	BOOST_CHECK(checker(as<Sequence>("Abc"), as<Sequence>("aBc")));
	BOOST_CHECK(!checker(as<Sequence>("Abc"), as<Sequence>("bCd")));
}

BOOST_AUTO_TEST_SUITE_END();

}} // namespaces
