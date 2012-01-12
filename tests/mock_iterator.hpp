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

#ifndef UATRAITS_TESTS_MOCK_ITERATOR_HPP_INCLUDED
#define UATRAITS_TESTS_MOCK_ITERATOR_HPP_INCLUDED

#include "uatraits/config.hpp"
#include "uatraits/details/iterator.hpp"

namespace uatraits { namespace tests {

template <typename Iter>
class mock_iterator : public details::iterator<mock_iterator<Iter>, typename std::iterator_traits<Iter>::iterator_category, typename std::iterator_traits<Iter>::value_type> {

public:
	mock_iterator();
	mock_iterator(Iter iter);

	void increment();
	void decrement();
	bool equal(mock_iterator const &other) const;
	typename std::iterator_traits<Iter>::reference dereference();

	void advance(std::ptrdiff_t size);
	std::ptrdiff_t distance(mock_iterator<Iter> const &other) const;

private:
	Iter iter_;
};

template <typename Iter> inline
mock_iterator<Iter>::mock_iterator() :
	iter_()
{
}

template <typename Iter> inline 
mock_iterator<Iter>::mock_iterator(Iter iter) :
	iter_(iter)
{
}

template <typename Iter> inline void
mock_iterator<Iter>::increment() {
	++iter_;
}

template <typename Iter> inline void
mock_iterator<Iter>::decrement() {
	--iter_;
}

template <typename Iter> inline bool
mock_iterator<Iter>::equal(mock_iterator const &other) const {
	return iter_ == other.iter_;
}

template <typename Iter> inline typename std::iterator_traits<Iter>::reference
mock_iterator<Iter>::dereference() {
	return *iter_;
}

template <typename Iter> inline void
mock_iterator<Iter>::advance(std::ptrdiff_t size) {
	iter_ += size;
}

template <typename Iter> inline std::ptrdiff_t
mock_iterator<Iter>::distance(mock_iterator<Iter> const &other) const {
	return std::distance(iter_, other.iter_);
}

}} // namespaces

#endif // UATRAITS_TESTS_MOCK_ITERATOR_HPP_INCLUDED
