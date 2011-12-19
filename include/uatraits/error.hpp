// uatraits is a simple tool for user agent detection
// Copyright (C) 2011 Oleg Obolenskiy <highpower@yandex-team.ru>

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

#ifndef UATRAITS_ERROR_HPP_INCLUDED
#define UATRAITS_ERROR_HPP_INCLUDED

#include <string>
#include <exception>

#include "uatraits/config.hpp"

namespace uatraits {

class UATRAITS_API error : public std::exception {

public:
	error(char const *format, ...);
	virtual ~error() throw ();
	virtual char const* what() const throw ();
	
private:
	std::string message_;
};

inline
error::~error() throw () {
}

inline char const*
error::what() const throw () {
	return message_.c_str();
}

} // namespace

#endif // UATRAITS_ERROR_HPP_INCLUDED
