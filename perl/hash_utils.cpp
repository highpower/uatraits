#include "acsetup.hpp"
#include "hash_utils.hpp"

#include <strings.h>

#ifdef __cplusplus
extern "C" {
#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"
}
#endif // __cplusplus

namespace uatraits { namespace perl {

static const char true_str[] = "true";
static const char false_str[] = "false";

void
hash_value_set(void *hash_value, char const *key, std::size_t keylen, char const *value, std::size_t valuelen) {

	enum { TRUE_VALUE = 1, FALSE_VALUE = 0 };

	HV *hash = (HV*) hash_value;
	if ((sizeof(true_str) - 1) == valuelen && !strncasecmp(value, true_str, valuelen)) {
		hv_store(hash, key, keylen, newSViv(TRUE_VALUE), 0);
	}
	else if ((sizeof(false_str) - 1) == valuelen && !strncasecmp(value, false_str, valuelen)) {
		hv_store(hash, key, keylen, newSViv(FALSE_VALUE), 0);
	}
	else {
		hv_store(hash, key, keylen, newSVpv(value, valuelen), 0);
	}
}

}} // namespaces
