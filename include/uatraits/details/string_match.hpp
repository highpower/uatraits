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

#ifndef UATRAITS_DETAILS_STRING_MATCH_HPP_INCLUDED
#define UATRAITS_DETAILS_STRING_MATCH_HPP_INCLUDED

#include <iterator>

#include "uatraits/config.hpp"
#include "uatraits/shared.hpp"
#include "uatraits/shared_ptr.hpp"
#include "uatraits/details/type_traits.hpp"
#include "uatraits/details/static_assert.hpp"

namespace uatraits { namespace details {

template <typename Char, typename Traits>
class string_match_node;

template <typename Char, typename Traits>
class string_match {

public:
	string_match();
	virtual ~string_match();

	typedef Char char_type;
	typedef string_match<Char, Traits> type;

	void add(char_type const *value);
	template <typename Iter> void add(Iter begin, Iter end);

private:
	string_match(string_match const &);
	string_match& operator = (string_match const &);
	typedef string_match_node<Char, Traits> node_type;

private:
	shared_ptr<node_type> root_;
};

template <typename Char, typename Traits>
class string_match_node : public shared {

public:
	string_match_node();
	
private:

};

}} // namespaces

#endif // UATRAITS_DETAILS_STRING_MATCH_HPP_INCLUDED
