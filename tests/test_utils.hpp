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

#ifndef UATRAITS_TESTS_TEST_UTILS_HPP_INCLUDED
#define UATRAITS_TESTS_TEST_UTILS_HPP_INCLUDED

#include <list>
#include <string>
#include <vector>
#include <boost/mpl/list.hpp>

#include "uatraits/config.hpp"
#include "uatraits/details/range.hpp"

namespace uatraits { namespace tests {

template <typename Sequence> inline Sequence
as(char *str) {
	return Sequence(str, str + std::char_traits<char>::length(str));
}

template <typename Sequence> inline Sequence
as(char const *str) {
	return Sequence(str, str + std::char_traits<char>::length(str));
}

template <typename Sequence> inline std::string
make_string(Sequence const &seq) {
	return std::string(seq.begin(), seq.end());
}

typedef boost::mpl::list<std::list<char>, std::vector<char>, details::range<char const*>, std::string> test_param_list;

}} // namespaces

#endif // UATRAITS_TESTS_TEST_UTILS_HPP_INCLUDED
