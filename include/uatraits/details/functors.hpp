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


#ifndef UATRAITS_DETAILS_FUNCTORS_HPP_INCLUDED
#define UATRAITS_DETAILS_FUNCTORS_HPP_INCLUDED

#include <algorithm>
#include <functional>

#include "uatraits/config.hpp"
#include "uatraits/details/char_traits.hpp"
#include "uatraits/details/type_traits.hpp"

namespace uatraits { namespace details {

template <typename Pred, typename T>
struct unary_predicate : public std::unary_function<T, bool> {
	bool operator () (T var) const;
};

template <typename Pred, typename T>
struct binary_predicate : public std::binary_function<T const&, T const&, bool> {
	bool operator () (T const &var, T const &target) const;
};

template <typename Char>
struct is_equal : public std::unary_function<Char, bool> {
	is_equal(Char pattern);
	bool operator () (Char value) const;
	typedef is_equal<Char> type;

private:
	Char value_;
};

template <typename Char>
struct is_space : public unary_predicate<is_space<Char>, Char> {
	typedef is_space<Char> type;
	static bool check(Char value);
};

template <typename Char>
struct is_numeric : public unary_predicate<is_numeric<Char>, Char> {
	typedef is_numeric<Char> type;
	static bool check(Char value);
};

template <typename Char>
struct is_line_end : public unary_predicate<is_line_end<Char>, Char> {
	typedef is_line_end<Char> type;
	static bool check(Char value);
};

template <typename Char>
struct is_not_line_end : public unary_predicate<is_not_line_end<Char>, Char> {
	typedef is_not_line_end<Char> type;
	static bool check(Char value);
};

template <typename Sequence>
struct ci_less : public binary_predicate<ci_less<Sequence>, Sequence> {
	typedef ci_less<Sequence> type;
	static bool check(Sequence const &var, Sequence const &target);
};

template <typename Sequence>
struct ci_equal : public binary_predicate<ci_equal<Sequence>, Sequence> {
	typedef ci_equal<Sequence> type;
	static bool check(Sequence const &var, Sequence const &target);
};

template <>
struct ci_less<char> : public std::binary_function<char, char, bool> {
	typedef ci_less<char> type;
	bool operator () (char var, char target) const;
};

template <>
struct ci_equal<char> : public std::binary_function<char, char, bool> {
	typedef ci_equal<char> type;
	bool operator () (char var, char target) const;
};

template <typename Pred, typename T> inline bool
unary_predicate<Pred, T>::operator () (T value) const {
	return Pred::check(value);
}

template <typename Pred, typename T> inline bool
binary_predicate<Pred, T>::operator () (T const &var, T const &target) const {
	return Pred::check(var, target);
}

template <typename Char> inline
is_equal<Char>::is_equal(Char value) :
	value_(value)
{
}

template <typename Char> inline bool
is_equal<Char>::operator () (Char value) const {
	return value_ == value;
}

template <typename Char> inline bool
is_space<Char>::check(Char value) {
	return char_traits<typename remove_const<Char>::type>::is_space(value);
}

template <typename Char> inline bool
is_numeric<Char>::check(Char value) {
	return char_traits<typename remove_const<Char>::type>::is_numeric(value);
}

template <typename Char> inline bool
is_line_end<Char>::check(Char value) {
	return (static_cast<Char>('\n') == value || static_cast<Char>('\r') == value);
}

template <typename Char> inline bool
is_not_line_end<Char>::check(Char value) {
	return !is_line_end<Char>::check(value);
}

template <typename R1, typename R2> inline bool
is_ci_less(R1 const &var, R2 const &target) {
	return (!var.empty() && !target.empty()) ? std::lexicographical_compare(var.begin(), 
		var.end(), target.begin(), target.end(), ci_less<typename R1::value_type>()) : !target.empty();
}

template <typename R1, typename R2> inline bool
is_ci_equal(R1 const &var, R2 const &target) {
	if (var.size() == target.size()) {
		return var.empty() ? true : std::equal(var.begin(), 
			var.end(), target.begin(), ci_equal<typename R1::value_type>());
	}
	return false;
}

template <typename Sequence> inline bool
ci_less<Sequence>::check(Sequence const &var, Sequence const &target) {
	return is_ci_less(var, target);
}

template <typename Sequence> inline bool
ci_equal<Sequence>::check(Sequence const &var, Sequence const &target) {
	return is_ci_equal(var, target);
}

inline bool
ci_less<char>::operator () (char var, char target) const {
	return char_traits<char>::to_lower(var) < char_traits<char>::to_lower(target);
}

inline bool
ci_equal<char>::operator () (char var, char target) const {
	return char_traits<char>::to_lower(var) == char_traits<char>::to_lower(target);
}

inline bool
is_ci_equal_char(char var, char target) {
	return char_traits<char>::to_lower(var) == char_traits<char>::to_lower(target);
}

}} // namespaces

#endif // UATRAITS_DETAILS_FUNCTORS_HPP_INCLUDED
