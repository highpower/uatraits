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

#ifndef UATRAITS_TESTS_MOCK_SHARED_HPP_INCLUDED
#define UATRAITS_TESTS_MOCK_SHARED_HPP_INCLUDED

#include <set>

#include "uatraits/config.hpp"
#include "uatraits/shared.hpp"

namespace uatraits { namespace tests {

class mock_shared : public shared {

public:
	mock_shared();
	virtual ~mock_shared();
	
	static bool is_any_alive();
	static bool is_alive(mock_shared *object);

private:
	mock_shared(mock_shared const &);
	mock_shared& operator = (mock_shared const &);

private:
	static std::set<mock_shared*> objects_;
};

class mock_shared_derived : public mock_shared {

public:
	mock_shared_derived();
	virtual ~mock_shared_derived();

private:
	mock_shared_derived(mock_shared_derived const &);
	mock_shared_derived& operator = (mock_shared_derived const &);
};

}} // namespaces

#endif // UATRAITS_TESTS_MOCK_SHARED_HPP_INCLUDED
