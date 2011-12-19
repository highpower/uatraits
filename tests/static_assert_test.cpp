#include "acsetup.hpp"
#include "uatraits/details/static_assert.hpp"

namespace uatraits { namespace tests {

struct static_assert_test {
	UATRAITS_STATIC_ASSERT(1);
	UATRAITS_STATIC_ASSERT(true);
};

}} // namespaces
