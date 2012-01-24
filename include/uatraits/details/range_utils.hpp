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

#ifndef UATRAITS_DETAILS_RANGE_UTILS_HPP_INCLUDED
#define UATRAITS_DETAILS_RANGE_UTILS_HPP_INCLUDED

#include <iostream>
#include <algorithm>

#include "uatraits/config.hpp"
#include "uatraits/details/range.hpp"

namespace uatraits { namespace details {

template <typename Iter, typename Char, typename Traits> inline std::basic_ostream<Char, Traits>&
operator << (std::basic_ostream<Char, Traits> &stream, range<Iter> const &value) {
	if (!value.empty()) {
		std::copy(value.begin(), value.end(), std::ostream_iterator<typename range<Iter>::value_type, Char, Traits>(stream));
	}
	return stream;
}

}} // namespaces

#endif // UATRAITS_DETAILS_RANGE_UTILS_HPP_INCLUDED
