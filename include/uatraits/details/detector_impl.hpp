// uatraits is a simple tool for user agent detection
// Copyright (C) 2011 Oleg Obolenskiy <highpower@yandex-team.ru>

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

#ifndef UATRAITS_DETAILS_DETECTOR_IMPL_HPP_INCLUDED
#define UATRAITS_DETAILS_DETECTOR_IMPL_HPP_INCLUDED

#include <cstring>

#include "uatraits/error.hpp"
#include "uatraits/config.hpp"
#include "uatraits/shared.hpp"
#include "uatraits/details/resource.hpp"
#include "uatraits/details/xml_elems.hpp"
#include "uatraits/details/xml_utils.hpp"

#include "uatraits/details/branch.hpp"
#include "uatraits/details/definition.hpp"
#include "uatraits/details/regex_definition.hpp"
#include "uatraits/details/static_definition.hpp"
#include "uatraits/details/string_definition.hpp"
#include "uatraits/details/complex_definition.hpp"

namespace uatraits { namespace details {

template <typename Traits>
class detector_impl : public shared {

public:
	detector_impl(xmlDocPtr doc);
	void detect(char const *begin, char const *end, Traits &traits) const;

private:
	detector_impl(detector_impl const &);
	detector_impl& operator = (detector_impl const &);

	typedef branch<Traits> branch_type;
	typedef definition<Traits> definition_type;

	void parse(xmlDocPtr doc);
	bool disabled(xmlNodePtr node) const;
	shared_ptr<branch_type> parse_branch(xmlNodePtr node) const;
	shared_ptr<definition_type> parse_definition(xmlNodePtr node) const;
	shared_ptr<definition_type> parse_complex_definition(char const *name, xmlNodePtr node) const;

private:
	shared_ptr<branch_type> root_;
};

template <typename Traits> inline 
detector_impl<Traits>::detector_impl(xmlDocPtr doc)
{
	root_.reset(new root_branch<Traits>());
	parse(doc);
}

template <typename Traits> inline void
detector_impl<Traits>::detect(char const *begin, char const *end, Traits &traits) const {
	root_->process(begin, end, traits);
}

template <typename Traits> inline void
detector_impl<Traits>::parse(xmlDocPtr doc)
{
	xmlNodePtr root = xmlDocGetRootElement(doc);
	if (0 == root) {
		throw error("got empty browser.xml");
	}
	xml_elems elems(root, "branch");
	for (xml_elems::iterator i = elems.begin(), end = elems.end(); i != end; ++i) {
		root_->add_child(parse_branch(*i));
	}
}

template <typename Traits> inline bool
detector_impl<Traits>::disabled(xmlNodePtr node) const {
	char const *value = xml_attr_text(node, "disabled");
	return value && (0 == strncasecmp(value, "true", sizeof("true")));
}

template <typename Traits> inline shared_ptr<typename detector_impl<Traits>::branch_type>
detector_impl<Traits>::parse_branch(xmlNodePtr node) const {
	
	shared_ptr<branch_type> result(new branch_type());
	for (xmlNodePtr n = xmlFirstElementChild(node); 0 != n; n = xmlNextElementSibling(n)) {
		if (disabled(n)) {
			continue;
		}
		else if (xmlStrncasecmp(n->name, (xmlChar const*) "match", sizeof("match")) == 0) {
			xml_elems elems(n, "pattern");
			for (xml_elems::iterator i = elems.begin(), end = elems.end(); i != end; ++i) {
				if (disabled(*i)) {
					continue;
				}
				char const *type = xml_attr_text(*i, "type");
				if (strncasecmp(type, "string", sizeof("string")) == 0) {
					result->add_match(xml_node_text(*i));
				}
				else if (strncasecmp(type, "regex", sizeof("regex")) == 0) {
					result->add_regex_match(xml_node_text(*i));
				}
				else {
					resource<xmlChar*, xml_string_traits> path(xmlGetNodePath(*i));
					throw error("unknown pattern type %s in [%s]", type, (char const*) path.get());
				}
			}
		}
		else if (xmlStrncasecmp(n->name, (xmlChar const*) "branch", sizeof("branch")) == 0) {
			result->add_child(parse_branch(n));
		}
		else if (xmlStrncasecmp(n->name, (xmlChar const*) "define", sizeof("definition")) == 0) {
			result->add_definition(parse_definition(n));
		}
	}
	return result;
}

template <typename Traits> inline shared_ptr<typename detector_impl<Traits>::definition_type>
detector_impl<Traits>::parse_definition(xmlNodePtr node) const {
	char const *name = xml_attr_text(node, "name"), *value = xml_attr_text(node, "value");
	if (static_cast<char const*>(0) == name) {
		resource<xmlChar*, xml_string_traits> path(xmlGetNodePath(node));
		throw error("definition without name in [%s]", (char const*) path.get());
	}
	else if (static_cast<char const*>(0) == value) {
		return parse_complex_definition(name, node);
	}
	else {
		return shared_ptr<definition_type>(new static_definition<Traits>(name, value));
	}
}

template <typename Traits> inline shared_ptr<typename detector_impl<Traits>::definition_type>
detector_impl<Traits>::parse_complex_definition(char const *name, xmlNodePtr node) const {

	typedef complex_definition<Traits> complex_type;
	shared_ptr<complex_type> parent(new complex_type(name));
	
	xml_elems elems(node, "pattern");
	for (xml_elems::const_iterator i = elems.begin(), end = elems.end(); i != end; ++i) {
		if (disabled(*i)) {
			continue;
		}
		char const *value = xml_attr_text(*i, "value"), *type = xml_attr_text(*i, "type");
		if (strncasecmp(type, "string", sizeof("string")) == 0) {
			parent->add(shared_ptr<definition_type>(new string_definition<Traits>(name, xml_node_text(*i), value)));
		}
		else if (strncasecmp(type, "regex", sizeof("regex")) == 0) {
			parent->add(shared_ptr<definition_type>(new regex_definition<Traits>(name, xml_node_text(*i), value)));
		}
		else {
			resource<xmlChar*, xml_string_traits> path(xmlGetNodePath(node));
			throw error("unknown pattern type %s in [%s]", type, (char const*) path.get());
		}
	}
	return parent->has_only_one() ? parent->release_child() : parent.template cast<definition_type>();
}

}} // namespaces

#endif // UATRAITS_DETAILS_DETECTOR_IMPL_HPP_INCLUDED
