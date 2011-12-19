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

#ifndef UATRAITS_DETAILS_INTEGER_HPP_INCLUDED
#define UATRAITS_DETAILS_INTEGER_HPP_INCLUDED

#include <limits>
#include <cstddef>

#include "uatraits/config.hpp"
#include "uatraits/details/type_list.hpp"
#include "uatraits/details/type_traits.hpp"
#include "uatraits/details/static_assert.hpp"

namespace uatraits { namespace details {

typedef UATRAITS_MAKE_TYPE_LIST5(signed long long, signed long, signed int, signed short, signed char) signed_integer_list;
typedef UATRAITS_MAKE_TYPE_LIST5(unsigned long long, unsigned long, unsigned int, unsigned short, unsigned char) unsigned_integer_list;

template <typename Arg>
struct is_int {
	static bool const result = (type_list_index_of<signed_integer_list, Arg>::result != -1) || (type_list_index_of<unsigned_integer_list, Arg>::result != -1);
};

template <>
struct is_int<char> {
	static bool const result = true;
};

template <std::size_t Size, typename List, bool IsFound>
struct integer_size_search_impl;

template <std::size_t Size, typename List>
struct integer_size_search {
	typedef typename integer_size_search_impl<Size, List, Size == sizeof(typename List::value_type)>::type type;
};

template <std::size_t Size, typename List>
struct integer_size_search_impl<Size, List, true> {
	typedef typename List::value_type type;
};

template <std::size_t Size, typename List>
struct integer_size_search_impl<Size, List, false> {
	typedef typename integer_size_search<Size, typename List::next_type>::type type;
};

template <std::size_t Size, bool IsFound>
struct integer_size_search_impl<Size, null_type, IsFound> {
	typedef null_type type;
};

typedef integer_size_search<1, signed_integer_list>::type int8_t;
typedef integer_size_search<2, signed_integer_list>::type int16_t;
typedef integer_size_search<4, signed_integer_list>::type int32_t;
typedef integer_size_search<8, signed_integer_list>::type int64_t;
typedef int64_t intmax_t;

typedef integer_size_search<1, unsigned_integer_list>::type uint8_t;
typedef integer_size_search<2, unsigned_integer_list>::type uint16_t;
typedef integer_size_search<4, unsigned_integer_list>::type uint32_t;
typedef integer_size_search<8, unsigned_integer_list>::type uint64_t;
typedef uint64_t uintmax_t;

template <bool IsSigned>
struct max_int_impl;

template <typename X>
struct max_int {
	typedef typename max_int_impl<std::numeric_limits<X>::is_signed>::type type;
};

template <>
struct max_int_impl<true> {
	typedef intmax_t type;
};

template <>
struct max_int_impl<false> {
	typedef uintmax_t type;
};

template <typename Arg>
struct make_signed {
	UATRAITS_STATIC_ASSERT(is_int<Arg>::result);
	static int const sp = type_list_index_of<signed_integer_list, Arg>::result;
	static int const up = type_list_index_of<unsigned_integer_list, Arg>::result;
	typedef typename type_list_nth_item<signed_integer_list, (-1 != sp) ? sp : up>::type type;
};

template <typename Arg> int const
make_signed<Arg>::sp;

template <typename Arg> int const
make_signed<Arg>::up;

template <>
struct make_signed<char> {
	typedef signed char type;
};

template <typename Arg>
struct make_unsigned {
	UATRAITS_STATIC_ASSERT(is_int<Arg>::result);
	static int const sp = type_list_index_of<signed_integer_list, Arg>::result;
	static int const up = type_list_index_of<unsigned_integer_list, Arg>::result;
	typedef typename type_list_nth_item<unsigned_integer_list, (-1 != sp) ? sp : up>::type type;
};

template <typename Arg> int const
make_unsigned<Arg>::sp;

template <typename Arg> int const
make_unsigned<Arg>::up;

template <>
struct make_unsigned<char> {
	typedef unsigned char type;
};

}} // namespaces

#endif // UATRAITS_DETAILS_INTEGER_HPP_INCLUDED
