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

#ifndef UATRAITS_SHARED_PTR_HPP_INCLUDED
#define UATRAITS_SHARED_PTR_HPP_INCLUDED

#include <cassert>
#include <algorithm>

#include "uatraits/config.hpp"

namespace uatraits {

template <typename T> 
class UATRAITS_API shared_ptr {

public:
	explicit shared_ptr(T *value = 0);
	virtual ~shared_ptr();
	
	shared_ptr(shared_ptr<T> const &rhs);
	template <typename Other> shared_ptr(shared_ptr<Other> const &rhs);
	
	shared_ptr<T>& operator = (shared_ptr<T> const &rhs);
	template <typename Other> shared_ptr<T>& operator = (shared_ptr<Other> const &rhs);

	T* get();
	T* operator -> ();
	
	T const* get() const;
	T const* operator -> () const;
	
	T& operator * ();
	T const& operator * () const;
	
	void reset(T *value = 0);
	void swap(shared_ptr<T> &ptr) throw ();
	
	class bool_convertible;
	operator bool_convertible const* () const;
	
	template <typename Other> friend class shared_ptr;
	template <typename Other> shared_ptr<Other> cast() const;

private:
	T *value_;
};

template <typename T> inline
shared_ptr<T>::shared_ptr(T *value) :
	value_(value)
{
	if (0 != value_) inc_ref(value_);
}

template <typename T> inline
shared_ptr<T>::~shared_ptr() {
	if (0 != value_) dec_ref(value_);
}

template <typename T> inline
shared_ptr<T>::shared_ptr(shared_ptr<T> const &rhs) :
	value_(rhs.value_)
{
	if (0 != value_) inc_ref(value_);
}

template <typename T> template <typename Other> inline 
shared_ptr<T>::shared_ptr(shared_ptr<Other> const &rhs) :
	value_(rhs.value_)
{
	if (0 != value_) inc_ref(value_);
}

template <typename T> inline shared_ptr<T>&
shared_ptr<T>::operator = (shared_ptr<T> const &rhs) {
	shared_ptr<T> tmp(rhs);
	swap(tmp);
	return *this;
}

template <typename T> template <typename Other> inline shared_ptr<T>&
shared_ptr<T>::operator = (shared_ptr<Other> const &rhs) {
	shared_ptr<T> tmp(rhs);
	swap(tmp);
	return *this;
}

template <typename T> inline T*
shared_ptr<T>::get() {
	return value_;
}

template <typename T> inline T*
shared_ptr<T>::operator -> () {
	assert(0 != value_);
	return value_;
}

template <typename T> inline T const*
shared_ptr<T>::get() const {
	return value_;
}

template <typename T> inline T const*
shared_ptr<T>::operator -> () const {
	assert(0 != value_);
	return value_;
}
	
template <typename T> inline T&
shared_ptr<T>::operator * () {
	assert(0 != value_);
	return *value_;
}

template <typename T> inline T const&
shared_ptr<T>::operator * () const {
	assert(0 != value_);
	return *value_;
}
	
template <typename T> inline void
shared_ptr<T>::reset(T *value) {
	shared_ptr<T> tmp(value);
	swap(tmp);
}

template <typename T> inline void
shared_ptr<T>::swap(shared_ptr<T> &ptr) throw () {
	std::swap(value_, ptr.value_);
}

template <typename T> inline 
shared_ptr<T>::operator typename shared_ptr<T>::bool_convertible const* () const {
	return (0 == value_) ? 0 : reinterpret_cast<bool_convertible const*>(value_);
}

template <typename T> template <typename Other> inline shared_ptr<Other>
shared_ptr<T>::cast() const {
	return shared_ptr<Other>(dynamic_cast<Other*>(value_));
}

template <typename T, typename Other> UATRAITS_API inline bool
operator == (shared_ptr<T> const &ptr, shared_ptr<Other> const &other) {
	return ptr.get() == other.get();
}

template <typename T, typename Other> UATRAITS_API inline bool
operator == (T const* const ptr, shared_ptr<Other> const &other) {
	return ptr == other.get();
}

template <typename T, typename Other> UATRAITS_API inline bool
operator == (shared_ptr<T> const &ptr, Other const* const other) {
	return ptr.get() == other;
}

template <typename T, typename Other> UATRAITS_API inline bool
operator != (shared_ptr<T> const &ptr, shared_ptr<Other> const &other) {
	return ptr.get() != other.get();
}

template <typename T, typename Other> UATRAITS_API inline bool
operator != (T const* const ptr, shared_ptr<Other> const &other) {
	return ptr != other.get();
}

template <typename T, typename Other> UATRAITS_API inline bool
operator != (shared_ptr<T> const &ptr, Other const* const other) {
	return ptr.get() != other;
}

template <typename T> UATRAITS_API inline void
swap(shared_ptr<T> &ptr, shared_ptr<T> &other) {
	ptr.swap(other);
}

} // namespace

#endif // UATRAITS_SHARED_PTR_HPP_INCLUDED
