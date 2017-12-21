#include "acsetup.hpp"
#include "uatraits/details/pcre_utils.hpp"

#include "uatraits/error.hpp"
#include "uatraits/details/resource.hpp"

#if (PCRE_MAJOR == 8) && (PCRE_MINOR >= 20)
#define UATRAITS_PCRE_FREE_STUDY pcre_free_study
#define UATRAITS_PCRE_STUDY_JIT_COMPILE PCRE_STUDY_JIT_COMPILE
#else
#define UATRAITS_PCRE_FREE_STUDY pcre_free
#define UATRAITS_PCRE_STUDY_JIT_COMPILE 0
#endif

namespace uatraits { namespace details {

struct pcre_traits {
	void destroy(pcre *value);
	static pcre* default_value();
};

struct pcre_extra_traits {
	void destroy(pcre_extra *extra);
	static pcre_extra* default_value();
};

inline void
pcre_traits::destroy(pcre *value) {
	pcre_free(value);
}

inline pcre*
pcre_traits::default_value() {
	return static_cast<pcre*>(0);
}

inline void
pcre_extra_traits::destroy(pcre_extra *extra) {
	UATRAITS_PCRE_FREE_STUDY(extra);
}

inline pcre_extra*
pcre_extra_traits::default_value() {
	return static_cast<pcre_extra*>(0);
}

std::pair<pcre*, pcre_extra*>
pcre_compile_regex(char const *pattern) {
	int error_offset = 0;
	char const *error_ptr = 0;
	resource<pcre*, pcre_traits> regex(pcre_compile(pattern, PCRE_CASELESS, &error_ptr, &error_offset, 0));
	if (!regex) {
		throw error("%s at %d of %s", error_ptr, error_offset, pattern);
	}
	resource<pcre_extra*, pcre_extra_traits> extra(pcre_study(regex.get(), UATRAITS_PCRE_STUDY_JIT_COMPILE, &error_ptr));
	if (!extra && error_ptr) {
		throw error("%s in %s", error_ptr, pattern);
	}
	return std::make_pair(regex.release(), extra.release());
}

void
pcre_free_regex(std::pair<pcre*, pcre_extra*> const &regex) {
	UATRAITS_PCRE_FREE_STUDY(regex.second);
	pcre_free(regex.first);
}

}} // namespaces
