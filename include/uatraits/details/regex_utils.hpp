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

#ifndef UATRAITS_DETAILS_REGEX_UTILS_HPP_INCLUDED
#define UATRAITS_DETAILS_REGEX_UTILS_HPP_INCLUDED

#include <string>
#include <iterator>

#include "uatraits/config.hpp"
#include "uatraits/details/functors.hpp"
#include "uatraits/details/char_traits.hpp"
#include "uatraits/details/string_utils.hpp"

namespace uatraits { namespace details {

struct regex_data {
	std::size_t begin;
	std::size_t end;
	std::size_t index;
};

template <typename Result, typename Iter> inline Result
scan_integer(Iter begin, Iter end) {
	Result result = 0;
	for (; begin != end; ++begin) {
		if (char_traits<typename std::iterator_traits<Iter>::value_type>::is_numeric(*begin)) {
			result *= 10;
			result += (*begin) - '0';
		}
		else {
			return result;
		}
	}
	return result;
}

template <typename Container> inline void
find_replaces(std::string const &value, Container &cont) {
	cont.clear();
	is_equal<char> dollar('$');
	is_numeric<char> numeric_matcher;
	std::string::const_iterator i = value.begin(), begin = value.begin(), end = value.end();
	while (i != end) {
		i = next_matched(i, end, dollar);
		std::string::const_iterator pos = next_not_matched(i + 1, end, numeric_matcher);
		if (std::distance(i, pos) > 1) {
			regex_data data = { i - begin, pos - begin, scan_integer<std::size_t>(i + 1, pos) };
			cont.push_back(data);
		}
		i = pos;
	}
}

}} // namespaces

#endif // UATRAITS_DETAILS_REGEX_UTILS_HPP_INCLUDED
