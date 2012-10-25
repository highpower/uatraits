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

#include <algorithm>
#include <cstring>
#include <iosfwd>
#include <map>
#include <string>

#include "uatraits/error.hpp"
#include "uatraits/config.hpp"
#include "uatraits/shared.hpp"
#include "uatraits/details/resource.hpp"
#include "uatraits/details/xml_elems.hpp"
#include "uatraits/details/xml_utils.hpp"

#include "uatraits/details/branch.hpp"
#include "uatraits/details/definition.hpp"
#include "uatraits/details/hash_utils.hpp"
#include "uatraits/details/regex_definition.hpp"
#include "uatraits/details/static_definition.hpp"
#include "uatraits/details/string_definition.hpp"
#include "uatraits/details/complex_definition.hpp"
#include "uatraits/details/detector_helper.hpp"

namespace uatraits { namespace details {

static const std::string X_OPERAMINI_PHONE_UA = "X-Operamini-Phone-Ua";
static const std::string X_WAP_PROFILE = "X-Wap-Profile";
static const std::string USER_AGENT = "User-Agent";

template <typename Traits>
class detector_impl : public shared {

public:
	detector_impl(xmlDocPtr doc);
	detector_impl(xmlDocPtr doc, xmlDocPtr profiles_doc);
	void detect(char const *begin, char const *end, Traits &traits) const;
	void detect(const Traits &headers, Traits &traits) const;
	void checked_detect(char const *begin, char const *end, Traits &traits, std::ostream &out) const;

	std::string const& date() const;

private:
	detector_impl(detector_impl const &);
	detector_impl& operator = (detector_impl const &);

	typedef branch<Traits> branch_type;
	typedef definition<Traits> definition_type;
	typedef std::map<std::string, std::map<std::string, std::string> > profiles_type;

	void parse(xmlDocPtr doc);
	void parse_profiles(xmlDocPtr doc);

	std::string find_header(const Traits &headers, const std::string &header) const;
	Traits detect_by_header(const std::string &header) const;
	void update_attr(Traits &traits, Traits &copy, const std::string &header) const;

	void store_headers(Traits &traits, const std::string &header) const;

	shared_ptr<branch_type> parse_branch(xmlNodePtr node) const;
	shared_ptr<definition_type> parse_definition(xmlNodePtr node) const;
	shared_ptr<definition_type> parse_complex_definition(char const *name, char const* xpath, xmlNodePtr node) const;

private:
	shared_ptr<branch_type> root_;
	std::string date_;
	profiles_type profiles_;
};

template <typename Traits> inline 
detector_impl<Traits>::detector_impl(xmlDocPtr doc)
{
	root_.reset(new root_branch<Traits>());
	parse(doc);
}

template <typename Traits> inline
detector_impl<Traits>::detector_impl(xmlDocPtr doc, xmlDocPtr profiles_doc)
{
	root_.reset(new root_branch<Traits>());
	parse(doc);
	parse_profiles(profiles_doc);
}

template <typename Traits> inline std::string const&
detector_impl<Traits>::date() const {
	return date_;
}

template <typename Traits> inline std::string
detector_impl<Traits>::find_header(const Traits &headers, const std::string &header) const {
	typename Traits::const_iterator it = headers.find(header);

        return (headers.end() != it ? it->second : "");
}

template <typename Traits> inline Traits
detector_impl<Traits>::detect_by_header(const std::string &header) const {
	Traits traits;

	detect(header.c_str(), header.c_str() + header.size(), traits);

	return traits;
}

template <typename Traits> inline void
detector_impl<Traits>::update_attr(Traits &traits, Traits &copy, const std::string &header) const {
	std::string value = find_header(copy, header);

	if (!value.empty()) {
		traits[header] = value;
	}
}

template <typename Traits> inline void
detector_impl<Traits>::store_headers(Traits &traits, const std::string &header) const {
	Traits copy = detect_by_header(header);

	update_attr(traits, copy, "isTablet");
	update_attr(traits, copy, "OSFamily");
	update_attr(traits, copy, "OSVersion");
	update_attr(traits, copy, "isMobile");
	update_attr(traits, copy, "isTouch");
}

template <typename Traits> inline void
detector_impl<Traits>::detect(char const *begin, char const *end, Traits &traits) const {
	root_->trigger(begin, end, traits);
}

template <typename Traits> inline void
detector_impl<Traits>::detect(const Traits &headers, Traits &traits) const {
	std::string agent = find_header(headers, USER_AGENT);

	if (agent.empty()) {
		return;
	}

	root_->trigger(agent.c_str(), agent.c_str() + agent.size(), traits);

	std::string header = find_header(headers, X_OPERAMINI_PHONE_UA);

	if (!header.empty()) {
		store_headers(traits, header);
	}

	header = find_header(headers, X_WAP_PROFILE);

	if (!header.empty()) {
		char to_strip[] = "\x22";

		header.erase(std::remove(header.begin(), header.end(), ' '), header.end());

		if (!header.compare(0, sizeof (to_strip) - 1, to_strip)) {
			header = header.substr(sizeof (to_strip) - 1, std::string::npos);
		}

		if (!header.compare(header.length() - sizeof (to_strip) + 1, std::string::npos, to_strip)) {
			header = header.substr(0, header.length() - sizeof (to_strip) + 1);
		}

		profiles_type::const_iterator pit = profiles_.find(md5(header));

		if (profiles_.end() != pit) {
			for (std::map<std::string, std::string>::const_iterator mit = pit->second.begin(),
				end = pit->second.end(); end != mit; ++mit) {
				traits[mit->first] = mit->second;
                        }
		}
	}
}

template <typename Traits> inline void
detector_impl<Traits>::parse(xmlDocPtr doc)
{
	xmlNodePtr root = xmlDocGetRootElement(doc);
	if (0 == root) {
		throw error("got empty browser.xml");
	}
	detector_helper::check_version(root);
	char const *date = xml_attr_text(root, "date");
	if (0 != date) {
		date_.assign(date);
	}

	xml_elems elems(root, "branch");
	for (xml_elems::iterator i = elems.begin(), end = elems.end(); i != end; ++i) {
		root_->add_child(parse_branch(*i));
	}
}

template <typename Traits> inline void
detector_impl<Traits>::parse_profiles(xmlDocPtr doc)
{
	xmlNodePtr root = xmlDocGetRootElement(doc);
	if (0 == root) {
		throw error("got empty browser.xml");
	}

	xml_elems elems(root, "profile");
	for (xml_elems::iterator i = elems.begin(), end = elems.end(); i != end; ++i) {
		char const *id = xml_attr_text(*i, "id");

		for (xmlNodePtr n = xmlFirstElementChild(*i); 0 != n; n = xmlNextElementSibling(n)) {
			if (xmlStrncasecmp(n->name, (xmlChar const*) "define", sizeof("define")) == 0) {
				char const *name = xml_attr_text(n, "name"), *value = xml_attr_text(n, "value");
				profiles_[id][name] = value;
			}
		}
	}
}

template <typename Traits> inline shared_ptr<typename detector_impl<Traits>::branch_type>
detector_impl<Traits>::parse_branch(xmlNodePtr node) const {
	
	resource<xmlChar*, xml_string_traits> path(xmlGetNodePath(node));
	shared_ptr<branch_type> result(new branch_type((char const*) path.get()));

	char const *type = xml_attr_text(node, "type");
	if (0 != type && strncasecmp(type, "common", sizeof("common")) == 0) {
		result->set_common(true);
	}
	else if (0 != type && strncasecmp(type, "default", sizeof("default")) == 0) {
		result->set_default(true);
	}
	
	for (xmlNodePtr n = xmlFirstElementChild(node); 0 != n; n = xmlNextElementSibling(n)) {
		if (detector_helper::disabled(n)) {
			continue;
		}
		if (xmlStrncasecmp(n->name, (xmlChar const*) "match", sizeof("match")) == 0) {
			xml_elems elems(n, "pattern");
			for (xml_elems::iterator i = elems.begin(), end = elems.end(); i != end; ++i) {
				if (detector_helper::disabled(*i)) {
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
					resource<xmlChar*, xml_string_traits> error_path(xmlGetNodePath(*i));
					throw error("unknown pattern type %s in [%s]", type, (char const*) error_path.get());
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
	resource<xmlChar*, xml_string_traits> path(xmlGetNodePath(node));
	char const *name = xml_attr_text(node, "name"), *value = xml_attr_text(node, "value");
	if (static_cast<char const*>(0) == name) {
		throw error("definition without name in [%s]", (char const*) path.get());
	}
	if (detector_helper::has_child_patterns(node)) {
		return parse_complex_definition(name, (char const*) path.get(), node);
	}
	if (static_cast<char const*>(0) == value) {
		value = xml_node_text(node);
	}
	return shared_ptr<definition_type>(new static_definition<Traits>(name, (char const*) path.get(), value));
}

template <typename Traits> inline shared_ptr<typename detector_impl<Traits>::definition_type>
detector_impl<Traits>::parse_complex_definition(char const *name, char const* xpath, xmlNodePtr node) const {

	typedef complex_definition<Traits> complex_type;
	shared_ptr<complex_type> parent(new complex_type(name, xpath));
	
	xml_elems elems(node, "pattern");
	for (xml_elems::const_iterator i = elems.begin(), end = elems.end(); i != end; ++i) {
		if (detector_helper::disabled(*i)) {
			continue;
		}
		resource<xmlChar*, xml_string_traits> path(xmlGetNodePath(*i));
		char const *value = xml_attr_text(*i, "value"), *type = xml_attr_text(*i, "type");
		
		if (strncasecmp(type, "string", sizeof("string")) == 0) {
			parent->add(shared_ptr<definition_type>(new string_definition<Traits>(name, (char const*) path.get(), xml_node_text(*i), value)));
		}
		else if (strncasecmp(type, "regex", sizeof("regex")) == 0) {
			parent->add(shared_ptr<definition_type>(new regex_definition<Traits>(name, (char const*) path.get(), xml_node_text(*i), value)));
		}
		else {
			throw error("unknown pattern type %s in [%s]", type, xpath);
		}
	}
	// return parent->has_only_one() ? parent->release_child() : parent.template cast<definition_type>();
	return parent.template cast<definition_type>();
}

}} // namespaces

#endif // UATRAITS_DETAILS_DETECTOR_IMPL_HPP_INCLUDED
