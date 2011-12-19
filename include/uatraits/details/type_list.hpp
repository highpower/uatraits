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

#ifndef UATRAITS_DETAILS_TYPE_LIST_HPP_INCLUDED
#define UATRAITS_DETAILS_TYPE_LIST_HPP_INCLUDED

#include <cstddef>

#include "uatraits/config.hpp"
#include "uatraits/details/type_traits.hpp"

namespace uatraits { namespace details {

template <typename T, typename Tail>
struct type_list {
	typedef T value_type;
	typedef Tail next_type;
	typedef type_list<T, Tail> type;
};

struct null_type {
};

template <typename List, typename X>
struct type_list_reverse_impl;

template <typename List>
struct type_list_reverse {
	typedef typename type_list_reverse_impl<List, null_type>::type type;
};

template <typename List, typename X>
struct type_list_reverse_impl {
	typedef typename type_list_reverse_impl<typename List::next_type, typename type_list<typename List::value_type, X>::type>::type type;
};

template <typename X>
struct type_list_reverse_impl<null_type, X> {
	typedef X type;
};

template <typename List, std::size_t N>
struct type_list_nth_item {
	typedef typename type_list_nth_item<typename List::next_type, N - 1>::type type;
};

template <typename List>
struct type_list_nth_item<List, static_cast<std::size_t>(0)> {
	typedef typename List::value_type type;
};

template <std::size_t N>
struct type_list_nth_item<null_type, N> {
	typedef null_type type;
};

template <typename X, typename List, int Index> 
struct type_list_index_of_impl;

template <typename List, typename X>
struct type_list_index_of {
	static int const result = type_list_index_of_impl<List, X, 0>::result;
};

template <typename List, typename X> int const
type_list_index_of<List, X>::result;

template <typename List, typename X, int Index> 
struct type_list_index_of_impl {
	static int const result = is_same<X, typename List::value_type>::result ? Index : type_list_index_of_impl<typename List::next_type, X, Index + 1>::result;
};

template <typename List, typename X, int Index> int const
type_list_index_of_impl<List, X, Index>::result;

template <typename X, int Index>
struct type_list_index_of_impl<null_type, X, Index> {
	static int const result = -1;
};

template <typename X, int Index> int const
type_list_index_of_impl<null_type, X, Index>::result;

}} // namespaces

#define UATRAITS_MAKE_TYPE_LIST1(T1) ::uatraits::details::type_list<T1, ::uatraits::details::null_type>
#define UATRAITS_MAKE_TYPE_LIST2(T1, T2) ::uatraits::details::type_list<T1, UATRAITS_MAKE_TYPE_LIST1(T2) >
#define UATRAITS_MAKE_TYPE_LIST3(T1, T2, T3) ::uatraits::details::type_list<T1, UATRAITS_MAKE_TYPE_LIST2(T2, T3) >
#define UATRAITS_MAKE_TYPE_LIST4(T1, T2, T3, T4) ::uatraits::details::type_list<T1, UATRAITS_MAKE_TYPE_LIST3(T2, T3, T4) >
#define UATRAITS_MAKE_TYPE_LIST5(T1, T2, T3, T4, T5) ::uatraits::details::type_list<T1, UATRAITS_MAKE_TYPE_LIST4(T2, T3, T4, T5) >
#define UATRAITS_MAKE_TYPE_LIST6(T1, T2, T3, T4, T5, T6) ::uatraits::details::type_list<T1, UATRAITS_MAKE_TYPE_LIST5(T2, T3, T4, T5, T6) >
#define UATRAITS_MAKE_TYPE_LIST7(T1, T2, T3, T4, T5, T6, T7) ::uatraits::details::type_list<T1, UATRAITS_MAKE_TYPE_LIST6(T2, T3, T4, T5, T6, T7) >
#define UATRAITS_MAKE_TYPE_LIST8(T1, T2, T3, T4, T5, T6, T7, T8) ::uatraits::details::type_list<T1, UATRAITS_MAKE_TYPE_LIST7(T2, T3, T4, T5, T6, T7, T8) >
#define UATRAITS_MAKE_TYPE_LIST9(T1, T2, T3, T4, T5, T6, T7, T8, T9) ::uatraits::details::type_list<T1, UATRAITS_MAKE_TYPE_LIST8(T2, T3, T4, T5, T6, T7, T8, T9) >

#endif // UATRAITS_DETAILS_TYPE_LIST_HPP_INCLUDED
