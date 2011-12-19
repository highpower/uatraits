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

#ifndef UATRAITS_DETAILS_CHAR_TRAITS_HPP_INCLUDED
#define UATRAITS_DETAILS_CHAR_TRAITS_HPP_INCLUDED

#include <cctype>

#include "uatraits/config.hpp"

namespace uatraits { namespace details {

template <typename Char>
struct char_traits;

template <>
struct char_traits<char> {
	
	static bool is_space(char value);
	static bool is_alpha(char value);
	static bool is_numeric(char value);
	static bool is_alpha_numeric(char value);

	static char to_upper(char value);
	static char to_lower(char value);
};

inline bool
char_traits<char>::is_space(char value) {
	return (' ' == value) || ('\t' == value);
}

inline bool
char_traits<char>::is_alpha(char value) {
	return isalpha(value);
}

inline bool
char_traits<char>::is_numeric(char value) {
	return isdigit(value);
}

inline bool
char_traits<char>::is_alpha_numeric(char value) {
	return isalnum(value);
}

inline char
char_traits<char>::to_upper(char value) {
	return toupper(value);
}

inline char
char_traits<char>::to_lower(char value) {
	return tolower(value);
}

}} // namespaces

#endif // UATRAITS_DETAILS_CHAR_TRAITS_HPP_INCLUDED
