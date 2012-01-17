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

#ifndef UATRAITS_DETAILS_PCRE_UTILS_HPP_INCLUDED
#define UATRAITS_DETAILS_PCRE_UTILS_HPP_INCLUDED

#include <pcre.h>
#include <utility>

#include "uatraits/config.hpp"

namespace uatraits { namespace details {

void pcre_free_regex(std::pair<pcre*, pcre_extra*> const &p);
std::pair<pcre*, pcre_extra*> pcre_compile_regex(char const *pattern);

}} // namespaces

#endif // UATRAITS_DETAILS_PCRE_UTILS_HPP_INCLUDED
