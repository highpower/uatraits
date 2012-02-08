#include "acsetup.hpp"
#include "uatraits/details/xml_utils.hpp"

#include <libxml/xmlerror.h>
#include "uatraits/error.hpp"

namespace uatraits { namespace details {

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

}} // namespaces
