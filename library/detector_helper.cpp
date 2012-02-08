#include "acsetup.hpp"
#include "uatraits/details/detector_helper.hpp"

#include <cstdio>
#include <strings.h>

#include "uatraits/error.hpp"
#include "uatraits/details/xml_utils.hpp"

namespace uatraits { namespace details {

struct detector_helper_init {
	detector_helper_init();
};

static int main_version = 0;
static int major_version = 0;

static detector_helper_init init;

detector_helper_init::detector_helper_init() {
	
	if (2 != sscanf(UATRAITS_PACKAGE_VERSION, "%d.%d", &main_version, &major_version)) {
		throw error("invalid package version: %s", UATRAITS_PACKAGE_VERSION);
	}
}

void
detector_helper::check_version(xmlNodePtr node) {

	char const *value = xml_attr_text(node, "minver");
	if (0 == value) {
		throw error("legacy format browser.xml, minver not found");
	}

	int main_minversion = 0, major_minversion = 0;
	if (2 != sscanf(value, "%d.%d", &main_minversion, &major_minversion)) {
		throw error("invalid format browser.xml, strange minver value: %s", value);
	}

	if (main_version < main_minversion || (main_version == main_minversion && major_version < major_minversion)) {
		throw error("old library version: " UATRAITS_PACKAGE_VERSION " < %s", value);
	}
}

bool
detector_helper::disabled(xmlNodePtr node) {
	char const *value = xml_attr_text(node, "disabled");
	return value && (0 == strncasecmp(value, "true", sizeof("true")));
}

bool
detector_helper::has_child_patterns(xmlNodePtr node) {
	for (xmlNodePtr current = node->children; 0 != current; current  = current->next) {
		if (XML_ELEMENT_NODE == current->type &&
			xmlStrncasecmp(current->name, (xmlChar const*) "pattern", sizeof("pattern")) == 0) {

			return true;
		}
	}
	return false;
}

}} // namespaces
