#ifdef __cplusplus

extern "C" {

#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#include "ppport.h"
#include "const-c.inc"

}
#endif // __cplusplus

#include <utility>
#include <exception>
#include "detector.hpp"

namespace uatraits { namespace perl {

class detector_wrapper {

public:
	detector_wrapper(char *name);
	virtual ~detector_wrapper();
	SV* detect(char *value);

private:
	detector detector_;
};

detector_wrapper::detector_wrapper(char *name) :
	detector_(name)
{
}

detector_wrapper::~detector_wrapper() {
}

SV*
detector_wrapper::detect(char *value) {
	typedef std::pair<char const*, char const*> item;
	enumeration<item>::pointer en = detector_.detect(value, value + strlen(value));
}

}} // namespaces

using uatraits::perl::detector_wrapper;

MODULE = uatraits		PACKAGE = uatraits		

INCLUDE: const-xs.inc

PROTOTYPES: ENABLED

detector_wrapper*
detector_wrapper::new(CLASS, name)
		char* name
	CODE:
		try {
			RETVAL = new detector_wrapper(name);
		}
		catch (std::exception const &e) {
			croak("%s", e.what());
		}
	OUTPUT:
		RETVAL

void
detector_wrapper::DESTROY()

SV*
detector_wrapper::detect(value)
		char *value
	CODE:
		RETVAL = THIS->detect(value);
	OUTPUT:
		RETVAL
