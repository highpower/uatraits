// uatraits - modern web framework for Python and C++
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

#ifndef UATRAITS_DETAILS_TYPE_TRAITS_HPP_INCLUDED
#define UATRAITS_DETAILS_TYPE_TRAITS_HPP_INCLUDED

#include "uatraits/config.hpp"

namespace uatraits { namespace details {

struct true_type {
	static bool const result = true;
};

struct false_type {
	static bool const result = false;
};

template <typename Arg, typename Other>
struct is_same : public false_type {
};

template <typename Arg>
struct is_same<Arg, Arg> : public true_type {
};

template <typename Arg>
struct is_const : public false_type {
};

template <typename Arg>
struct is_const<Arg const> : public true_type {
};

template <typename Arg>
struct is_pointer : public false_type {
};

template <typename Arg>
struct is_pointer<Arg*> : public true_type {
};

template <typename Arg>
struct add_const {
	typedef Arg const type;
};

template <typename Arg>
struct add_const<Arg const> {
	typedef Arg const type;
};

template <typename Arg>
struct remove_const {
	typedef Arg type;
};

template <typename Arg>
struct remove_const<Arg const> {
	typedef Arg type;
};

}} // namespaces

#endif // UATRAITS_DETAILS_TYPE_TRAITS_HPP_INCLUDED
