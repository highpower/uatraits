#include "acsetup.hpp"
#include "uatraits/details/xml_utils.hpp"

#include <libxml/xmlerror.h>
#include "uatraits/error.hpp"

namespace uatraits { namespace details {

struct xml_setup {
	xml_setup();
};

void
xml_throw_unless(bool condition) {
	if (!condition) {
		char const *message = "unknown XML error";
		xmlErrorPtr xmlErr = xmlGetLastError();
		if (xmlErr && xmlErr->message) {
			message = xmlErr->message;
		}
		error err("%s", message);
		xmlResetLastError();
		throw err;
	}
}

extern "C" void
uatraits_xml_null_error(void *ctx, char const *format, ...) {
	(void) ctx;
	(void) format;
}

xml_setup::xml_setup() {
	xmlSubstituteEntitiesDefault(1);
	xmlSetGenericErrorFunc(0, &uatraits_xml_null_error);
}

static xml_setup setup_;

}} // namespaces
