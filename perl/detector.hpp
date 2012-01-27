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

#ifndef UATRAITS_PERL_DETECTOR_HPP_INCLUDED
#define UATRAITS_PERL_DETECTOR_HPP_INCLUDED

#include <utility>

#include "uatraits/config.hpp"
#include "uatraits/forward.hpp"
#include "uatraits/shared_ptr.hpp"

namespace uatraits { namespace perl {

class hash_wrapper;

class detector {

public:
	detector(char const *name);
	virtual ~detector();
	void detect(char const *begin, char const *end, void *hv) const;

private:
	typedef details::detector_impl<hash_wrapper> impl_type;
	shared_ptr<impl_type> impl_;
};

}} // namespaces

#endif // UATRAITS_PERL_DETECTOR_HPP_INCLUDED
