// uatraits is a simple tool for user agent detection
// Copyright (C) 2011 Yandex <highpower@yandex-team.ru>

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#ifndef UATRAITS_DETAILS_XML_UTILS_HPP_INCLUDED
#define UATRAITS_DETAILS_XML_UTILS_HPP_INCLUDED

#include <cassert>
#include <libxml/tree.h>

#include "uatraits/config.hpp"

namespace uatraits { namespace details {

struct xml_doc_traits {
	void destroy(xmlDocPtr doc);
	static xmlDocPtr default_value();
};

struct xml_string_traits {
	void destroy(xmlChar *str);
	static xmlChar* default_value();
};

inline void
xml_doc_traits::destroy(xmlDocPtr doc) {
	xmlFreeDoc(doc);
}

inline xmlDocPtr
xml_doc_traits::default_value() {
	return static_cast<xmlDocPtr>(0);
}

inline void
xml_string_traits::destroy(xmlChar *str) {
	xmlFree(str);
}

inline xmlChar*
xml_string_traits::default_value() {
	return static_cast<xmlChar*>(0);
}

template <typename NodePtr> inline const char*
xml_node_text(NodePtr node) {
	assert(node);
	xmlNodePtr child = node->children;
	if (child && xmlNodeIsText(child) && child->content) {
		return (const char*) child->content;
	}
	return static_cast<char const*>(0);
}

inline const char*
xml_attr_text(xmlNodePtr node, const char *name) {
	assert(node);
	xmlAttrPtr attr = xmlHasProp(node, (const xmlChar*) name);
	return attr ? xml_node_text(attr) : static_cast<char const*>(0);
}

void
xml_throw_unless(bool condition);

}} // namespaces

#endif // UATRAITS_DETAILS_XML_UTILS_HPP_INCLUDED
