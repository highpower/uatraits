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

#ifndef UATRAITS_ENUMERATION_HPP_INCLUDED
#define UATRAITS_ENUMERATION_HPP_INCLUDED

#include "uatraits/config.hpp"
#include "uatraits/shared.hpp"
#include "uatraits/shared_ptr.hpp"

namespace uatraits {

template <typename Item>
class enumeration : public shared {

public:
	enumeration();
	virtual ~enumeration();

	typedef Item value_type;
	typedef enumeration<value_type> type;
	typedef shared_ptr<type> pointer;

	virtual Item next() const = 0;
	virtual bool empty() const = 0;

private:
	enumeration(enumeration const &);
	enumeration& operator = (enumeration const &);
};

template <typename Item> inline
enumeration<Item>::enumeration() {
}

template <typename Item> inline
enumeration<Item>::~enumeration() {
}

} // namespace

#endif // UATRAITS_ENUMERATION_HPP_INCLUDED
