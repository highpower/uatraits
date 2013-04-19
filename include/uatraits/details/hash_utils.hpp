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

#ifndef UATRAITS_DETAILS_RANGE_UTILS_HPP_INCLUDED
#define UATRAITS_DETAILS_RANGE_UTILS_HPP_INCLUDED

#include <string>

#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/md5.h>

namespace uatraits { namespace details {

static std::string
md5(const std::string &source) {
	MD5_CTX md5handler;
	unsigned char md5digest[MD5_DIGEST_LENGTH];
	MD5_Init(&md5handler);
	MD5_Update(&md5handler, source.c_str(), source.length());
	MD5_Final(md5digest, &md5handler);
	std::string hash;
	for (std::size_t i = 0; i < MD5_DIGEST_LENGTH; ++i) {
		char hex[3];
		snprintf(hex, sizeof (hex), "%02x", md5digest[i]);
		hash.append(hex);
	}
	return hash;
}

}} // namespace

#endif // UATRAITS_DETAILS_RANGE_UTILS_HPP_INCLUDED
