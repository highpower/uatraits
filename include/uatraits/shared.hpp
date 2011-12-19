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

#ifndef UATRAITS_SHARED_HPP_INCLUDED
#define UATRAITS_SHARED_HPP_INCLUDED

#include <cassert>

#include "uatraits/config.hpp"

namespace uatraits {

class shared;

void inc_ref(shared *object);
void dec_ref(shared *object);

class UATRAITS_API shared {

public:
	shared();
	virtual ~shared();

private:
	shared(shared const &);
	shared& operator = (shared const &);
	friend void inc_ref(shared *object);
	friend void dec_ref(shared *object);

private:
	int count_;
};

inline
shared::shared() :
	count_(0)
{
}

inline
shared::~shared() {
	assert(0 == count_);
}

UATRAITS_API inline void
inc_ref(shared *object) {
	++object->count_;
}

UATRAITS_API inline void
dec_ref(shared *object) {
	if (0 == --object->count_) delete object;
}

} // namespace

#endif // UATRAITS_SHARED_HPP_INCLUDED
