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

#ifndef UATRAITS_DETAILS_REGEX_TRAITS_HPP_INCLUDED
#define UATRAITS_DETAILS_REGEX_TRAITS_HPP_INCLUDED

#include <pcre.h>

#include "uatraits/config.hpp"
#include "uatraits/details/range.hpp"

namespace uatraits { namespace details {

struct pcre_traits {
	void destroy(pcre *value);
	static pcre* default_value();
};

struct pcre_extra_traits {
	void destroy(pcre_extra *extra);
	static pcre_extra* default_value();
};

inline void
pcre_traits::destroy(pcre *value) {
	pcre_free(value);
}

inline pcre*
pcre_traits::default_value() {
	return static_cast<pcre*>(0);
}

inline void
pcre_extra_traits::destroy(pcre_extra *extra) {
	pcre_free_study(extra);
}

inline pcre_extra*
pcre_extra_traits::default_value() {
	return static_cast<pcre_extra*>(0);
}

}} // namespaces

#endif // UATRAITS_DETAILS_REGEX_TRAITS_HPP_INCLUDED
