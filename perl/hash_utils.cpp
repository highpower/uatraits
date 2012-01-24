#include "acsetup.hpp"
#include "hash_utils.hpp"

#ifdef __cplusplus
extern "C" {
#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"
}
#endif // __cplusplus

namespace uatraits { namespace perl {

void
hash_value_set(void *hash_value, char const *key, std::size_t keylen, char const *value, std::size_t valuelen) {
	HV *hash = (HV*) hash_value;
	hv_store(hash, key, keylen, newSVpv(value, valuelen), 0);
}

}} // namespaces
