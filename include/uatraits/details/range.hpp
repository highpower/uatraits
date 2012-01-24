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

#ifndef UATRAITS_DETAILS_RANGE_HPP_INCLUDED
#define UATRAITS_DETAILS_RANGE_HPP_INCLUDED

#include <cstring>
#include <cassert>
#include <iterator>
#include <algorithm>
#include <functional>

#include "uatraits/config.hpp"
#include "uatraits/details/integer.hpp"

namespace uatraits { namespace details {

template <typename Iter, typename Tag>
class range_base;

template <typename Iter>
class range_base<Iter, std::forward_iterator_tag> {

public:
	range_base();
	range_base(Iter begin, Iter end);
	
	typedef Iter iterator;
	typedef Iter const_iterator;
	
	typedef typename std::iterator_traits<Iter>::pointer pointer;
	typedef typename std::iterator_traits<Iter>::value_type value_type;

	typedef typename std::iterator_traits<Iter>::reference reference;
	typedef typename std::iterator_traits<Iter>::reference const const_reference;

	typedef typename std::iterator_traits<Iter>::difference_type difference_type;
	typedef typename make_unsigned<typename std::iterator_traits<Iter>::difference_type>::type size_type;

	iterator end();
	const_iterator end() const;

	iterator begin();
	const_iterator begin() const;

	void clear();
	bool empty() const;
	size_type size() const;

private:
	bool empty_;
	Iter begin_, end_;
};

template <typename Iter>
class range_base<Iter, std::bidirectional_iterator_tag> : public range_base<Iter, std::forward_iterator_tag> {

public:
	range_base();
	range_base(Iter begin, Iter end);

	typedef range_base<Iter, std::forward_iterator_tag> base_type;

	typedef typename base_type::iterator iterator;
	typedef typename base_type::const_iterator const_iterator;
	
	typedef typename base_type::pointer pointer;
	typedef typename base_type::value_type value_type;

	typedef typename base_type::reference reference;
	typedef typename base_type::reference const const_reference;

	typedef typename base_type::difference_type difference_type;
	typedef typename base_type::size_type size_type;

	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

	using base_type::end;
	using base_type::begin;
	
	reverse_iterator rend();
	const_reverse_iterator rend() const;

	reverse_iterator rbegin();
	const_reverse_iterator rbegin() const;
};

template <typename Iter>
class range_base<Iter, std::random_access_iterator_tag> : public range_base<Iter, std::bidirectional_iterator_tag> {

public:
	range_base();
	range_base(Iter begin, Iter end);
	
	typedef range_base<Iter, std::bidirectional_iterator_tag> base_type;

	typedef typename base_type::iterator iterator;
	typedef typename base_type::const_iterator const_iterator;
	
	typedef typename base_type::pointer pointer;
	typedef typename base_type::value_type value_type;

	typedef typename base_type::reference reference;
	typedef typename base_type::reference const const_reference;

	typedef typename base_type::difference_type difference_type;
	typedef typename base_type::size_type size_type;

	typedef typename base_type::reverse_iterator reverse_iterator;
	typedef typename base_type::const_reverse_iterator const_reverse_iterator;

	const_reference operator [] (size_type index) const;
};

template <typename Iter>
class range : public range_base<Iter, typename std::iterator_traits<Iter>::iterator_category> {

public:
	range();
	range(Iter begin, Iter end);
};

template <typename Iter> inline
range_base<Iter, std::forward_iterator_tag>::range_base() :
	empty_(true), begin_(), end_()
{
}

template <typename Iter> inline
range_base<Iter, std::forward_iterator_tag>::range_base(Iter begin, Iter end) :
	empty_(false), begin_(begin), end_(end)
{
}

template <typename Iter> inline typename range_base<Iter, std::forward_iterator_tag>::iterator 
range_base<Iter, std::forward_iterator_tag>::end() {
	assert(!empty_);
	return end_;
}

template <typename Iter> inline typename range_base<Iter, std::forward_iterator_tag>::const_iterator
range_base<Iter, std::forward_iterator_tag>::end() const {
	assert(!empty_);
	return end_;
}

template <typename Iter> inline typename range_base<Iter, std::forward_iterator_tag>::iterator 
range_base<Iter, std::forward_iterator_tag>::begin() {
	assert(!empty_);
	return begin_;
}

template <typename Iter> inline typename range_base<Iter, std::forward_iterator_tag>::const_iterator
range_base<Iter, std::forward_iterator_tag>::begin() const {
	assert(!empty_);
	return begin_;
}

template <typename Iter> inline void
range_base<Iter, std::forward_iterator_tag>::clear() {
	begin_ = end_;
	empty_ = true;
}

template <typename Iter> inline bool
range_base<Iter, std::forward_iterator_tag>::empty() const {
	return empty_ ? true : (begin_ == end_);
}

template <typename Iter> inline typename range_base<Iter, std::forward_iterator_tag>::size_type
range_base<Iter, std::forward_iterator_tag>::size() const {
	return empty() ? 0 : std::distance(begin_, end_);
}

template <typename Iter> inline 
range_base<Iter, std::bidirectional_iterator_tag>::range_base() :
	base_type()
{
}

template <typename Iter> inline 
range_base<Iter, std::bidirectional_iterator_tag>::range_base(Iter begin, Iter end) :
	base_type(begin, end)
{
}

template <typename Iter> inline typename range_base<Iter, std::bidirectional_iterator_tag>::reverse_iterator
range_base<Iter, std::bidirectional_iterator_tag>::rend() {
	return reverse_iterator(begin());
}

template <typename Iter> inline typename range_base<Iter, std::bidirectional_iterator_tag>::const_reverse_iterator
range_base<Iter, std::bidirectional_iterator_tag>::rend() const {
	return const_reverse_iterator(begin());
}

template <typename Iter> inline typename range_base<Iter, std::bidirectional_iterator_tag>::reverse_iterator
range_base<Iter, std::bidirectional_iterator_tag>::rbegin() {
	return reverse_iterator(end());
}

template <typename Iter> inline typename range_base<Iter, std::bidirectional_iterator_tag>::const_reverse_iterator
range_base<Iter, std::bidirectional_iterator_tag>::rbegin() const {
	return const_reverse_iterator(end());
}

template <typename Iter> inline 
range_base<Iter, std::random_access_iterator_tag>::range_base() :
	range_base<Iter, std::bidirectional_iterator_tag>()
{
}

template <typename Iter> inline 
range_base<Iter, std::random_access_iterator_tag>::range_base(Iter begin, Iter end) :
	range_base<Iter, std::bidirectional_iterator_tag>(begin, end)
{
}

template <typename Iter> inline typename range_base<Iter, std::random_access_iterator_tag>::const_reference
range_base<Iter, std::random_access_iterator_tag>::operator [] (typename range_base<Iter, typename std::random_access_iterator_tag>::size_type index) const {
	return *(this->begin() + index);
}

template <typename Iter> inline
range<Iter>::range() :
	range_base<Iter, typename std::iterator_traits<Iter>::iterator_category>()
{
}

template <typename Iter> inline
range<Iter>::range(Iter begin, Iter end) :
	range_base<Iter, typename std::iterator_traits<Iter>::iterator_category>(begin, end)
{
}

template <typename Iter, typename Other> inline bool
operator < (range<Iter> const &value, range<Other> const &other) {
	return (!value.empty() && !other.empty()) ? std::lexicographical_compare(value.begin(), value.end(), other.begin(), other.end()) : !other.empty();
}

template <typename Iter, typename Other> inline bool
operator > (range<Iter> const &value, range<Other> const &other) {
	std::greater<typename std::iterator_traits<Iter>::value_type> pred;
	return (!value.empty() && !other.empty()) ? std::lexicographical_compare(value.begin(), value.end(), other.begin(), other.end(), pred) : !value.empty();
}

template <typename Iter, typename Other> inline bool
operator == (range<Iter> const &value, range<Other> const &other) {
	if (value.size() == other.size()) {
		return value.empty() ? true : std::equal(value.begin(), value.end(), other.begin());
	}
	return false;
}

template <typename Iter, typename Other> inline bool
operator != (range<Iter> const &value, range<Other> const &other) {
	return !operator == (value, other);
}

template <typename Iter, typename Other> inline bool
operator <= (range<Iter> const &value, range<Other> const &other) {
	return !operator > (value, other);
}

template <typename Iter, typename Other> inline bool
operator >= (range<Iter> const &value, range<Other> const &other) {
	return !operator < (value, other);
}

template <typename Sequence> inline range<typename Sequence::const_iterator> 
make_range(Sequence const &seq) {
	return range<typename Sequence::const_iterator>(seq.begin(), seq.end());
}

template <typename Iter> inline range<Iter>
make_range(Iter begin, Iter end) {
	return range<Iter>(begin, end);
}

inline range<char*>
make_range(char *str) {
	return range<char*>(str, str + strlen(str));
}

inline range<char*>
make_range(char *str, std::size_t size) {
	return range<char*>(str, str + size);
}

inline range<char const*>
make_range(char const *str) {
	return range<char const*>(str, str + strlen(str));
}

inline range<char const*>
make_range(char const *str, std::size_t size) {
	return range<char const*>(str, str + size);
}

}} // namespaces

#endif // UATRAITS_DETAILS_RANGE_HPP_INCLUDED
