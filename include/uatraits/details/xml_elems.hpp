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

#ifndef UATRAITS_DETAILS_XML_ELEMENT_SET_HPP_INCLUDED
#define UATRAITS_DETAILS_XML_ELEMENT_SET_HPP_INCLUDED

#include <string>
#include <iterator>
#include <libxml/tree.h>

#include "uatraits/config.hpp"
#include "uatraits/details/range.hpp"
#include "uatraits/details/iterator.hpp"
#include "uatraits/details/xml_utils.hpp"

namespace uatraits { namespace details {

class xml_elems_iterator : public ::uatraits::details::iterator<xml_elems_iterator, std::forward_iterator_tag, xmlNodePtr const> {
	
public:
	xml_elems_iterator();
	xml_elems_iterator(xmlNodePtr node, range<char const*> const &name);
	
	void increment();
	xmlNodePtr const& dereference();
	bool equal(xml_elems_iterator const &other) const;
	
private:
	xmlNodePtr current_;
	range<char const*> name_;
};

class xml_elems {

public:
	xml_elems(xmlNodePtr parent, char const *name);
	xml_elems(xmlNodePtr parent, std::string const &name);
	
	typedef xmlNodePtr value_type;
	typedef xml_elems_iterator iterator;
	typedef xml_elems_iterator const_iterator;
	
	iterator end();
	iterator begin();
	const_iterator end() const;
	const_iterator begin() const;
	
private:
	xmlNodePtr parent_;
	std::string element_name_;
};

inline 
xml_elems_iterator::xml_elems_iterator() :
	current_(0), name_()
{
}

inline 
xml_elems_iterator::xml_elems_iterator(xmlNodePtr node, range<char const*> const &name) :
	current_(node), name_(name)
{
}

inline void
xml_elems_iterator::increment() {
	while (true) {
		current_ = xmlNextElementSibling(current_);
		if (!current_ || xmlStrncasecmp(current_->name, (xmlChar const*) name_.begin(), name_.size()) == 0) {
			return;
		}
	}
}

inline xmlNodePtr const&
xml_elems_iterator::dereference() {
	assert(current_);
	return current_;
}

inline bool
xml_elems_iterator::equal(xml_elems_iterator const &other) const {
	return (current_ == other.current_) && (name_ == other.name_);
}

inline
xml_elems::xml_elems(xmlNodePtr parent, char const *name) :
	parent_(parent), element_name_(name)
{
}

inline
xml_elems::xml_elems(xmlNodePtr parent, std::string const &name) :
	parent_(parent), element_name_(name)
{
}

inline xml_elems::iterator
xml_elems::end() {
	return iterator(0, make_range(element_name_.c_str(), element_name_.size()));
}

inline xml_elems::iterator
xml_elems::begin() {
	return iterator(xmlFirstElementChild(parent_), make_range(element_name_.c_str(), element_name_.size()));
}

inline xml_elems::const_iterator
xml_elems::end() const {
	return const_iterator(0, make_range(element_name_.c_str(), element_name_.size()));
}

inline xml_elems::const_iterator
xml_elems::begin() const {
	return const_iterator(xmlFirstElementChild(parent_), make_range(element_name_.c_str(), element_name_.size()));
}

}} // namespaces

#endif // UATRAITS_DETAILS_XML_ELEMENT_SET_HPP_INCLUDED
