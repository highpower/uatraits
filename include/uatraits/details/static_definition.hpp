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

#ifndef UATRAITS_DETAILS_STATIC_DEFINITION_HPP_INCLUDED
#define UATRAITS_DETAILS_STATIC_DEFINITION_HPP_INCLUDED

#include <string>
#include <iostream>

#include "uatraits/config.hpp"
#include "uatraits/details/definition.hpp"

namespace uatraits { namespace details {

template <typename Traits>
class static_definition : public definition<Traits> {
	
public:
	static_definition(char const *name, char const *xpath, char const *value);
	virtual void dump(std::ostream &out) const;
	virtual bool detect(char const *begin, char const *end, Traits &traits) const;
	
private:
	static_definition(static_definition const &);
	static_definition& operator = (static_definition const &);
	using definition<Traits>::name;
	using definition<Traits>::xpath;

private:
	std::string value_;
};

template <typename Traits> inline
static_definition<Traits>::static_definition(char const *name, char const *xpath, char const *value) :
	definition<Traits>(name, xpath), value_(value)
{
}

template <typename Traits> inline void
static_definition<Traits>::dump(std::ostream &out) const {
	out << "static definition at [" << xpath() << "] triggered: setting " << name() << "=" << value_ << std::endl;
}

template <typename Traits> inline bool
static_definition<Traits>::detect(char const *begin, char const *end, Traits &traits) const {
	(void) begin; (void) end;
	traits[name()] = value_;
	return true;
}

}} // namespaces

#endif // UATRAITS_DETAILS_STATIC_DEFINITION_HPP_INCLUDED
