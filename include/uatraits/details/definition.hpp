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

#ifndef UATRAITS_DETAILS_DEFINITION_HPP_INCLUDED
#define UATRAITS_DETAILS_DEFINITION_HPP_INCLUDED

#include <list>
#include <string>
#include <iosfwd>

#include "uatraits/config.hpp"
#include "uatraits/shared.hpp"
#include "uatraits/details/range.hpp"

namespace uatraits { namespace details {

template <typename Traits>
class definition : public shared {

public:
	definition(char const *name, char const *xpath);
	virtual ~definition();

	std::string const& name() const;
	std::string const& xpath() const;

	virtual void dump(std::ostream &out) const = 0;
	virtual bool trigger(char const *begin, char const *end, Traits &traits) const = 0;
	virtual bool trigger_trace(char const *begin, char const *end, Traits &traits, std::ostream &out) const;

private:
	definition(definition const &);
	definition& operator = (definition const &);
	
private:
	std::string name_, xpath_;
}; 

template <typename Traits> inline
definition<Traits>::definition(char const *name, char const *xpath) :
	name_(name), xpath_(xpath)
{
}

template <typename Traits> inline 
definition<Traits>::~definition() {
}

template <typename Traits> inline std::string const&
definition<Traits>::name() const {
	return name_;
}

template <typename Traits> inline std::string const&
definition<Traits>::xpath() const {
	return xpath_;
}

template <typename Traits> inline bool
definition<Traits>::trigger_trace(char const *begin, char const *end, Traits &traits, std::ostream &out) const {
	if (trigger(begin, end, traits)) {
		dump(out);
		return true;
	}
	return false;
}

}} // namespaces

#endif // UATRAITS_DETAILS_DEFINITION_HPP_INCLUDED
