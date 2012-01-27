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

#ifndef UATRAITS_DETAILS_STRING_DEFINITION_HPP_INCLUDED
#define UATRAITS_DETAILS_STRING_DEFINITION_HPP_INCLUDED

#include <string>
#include <iostream>
#include <algorithm>

#include "uatraits/config.hpp"
#include "uatraits/details/definition.hpp"
#include "uatraits/details/functors.hpp"

namespace uatraits { namespace details {

template <typename Traits> 
class string_definition : public definition<Traits> {

public:
	string_definition(char const *name, char const *xpath, char const *pattern, char const *result);
	virtual void dump(std::ostream &out) const;
	virtual bool trigger(char const *begin, char const *end, Traits &traits) const;

private:
	string_definition(string_definition const &);
	string_definition& operator = (string_definition const &);
	using definition<Traits>::name;
	using definition<Traits>::xpath;

private:
	std::string pattern_, result_;
};

template <typename Traits> inline 
string_definition<Traits>::string_definition(char const *name, char const *xpath, char const *pattern, char const *result) :
	definition<Traits>(name, xpath), pattern_(pattern), result_(result)
{
}

template <typename Traits> inline void
string_definition<Traits>::dump(std::ostream &out) const {
	out << "string definition at [" << xpath() << "] triggered: setting " << name() << "=" << result_ << std::endl;
}

template <typename Traits> inline bool
string_definition<Traits>::trigger(char const *begin, char const *end, Traits &traits) const {
	if (std::search(begin, end, pattern_.begin(), pattern_.end(), is_ci_equal_char) != end) {
		traits[name()] = result_;
		return true;
	}
	return false;
}

}} // namespaces

#endif // UATRAITS_DETAILS_STRING_DEFINITION_HPP_INCLUDED
