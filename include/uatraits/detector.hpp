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

#ifndef UATRAITS_DETECTOR_HPP_INCLUDED
#define UATRAITS_DETECTOR_HPP_INCLUDED

#include <map>
#include <string>

#include "uatraits/config.hpp"
#include "uatraits/forward.hpp"
#include "uatraits/shared_ptr.hpp"

namespace uatraits {

class UATRAITS_API detector {

public:
	detector(char const *file);
	detector(char const *file, char const *profiles);
	virtual ~detector();

	std::string const& date() const;

	typedef std::map<std::string, std::string> result_type;
	result_type detect(char const *agent) const;
	result_type detect(std::string const &agent) const;

	void detect(char const *agent, result_type &result) const;
	void detect(std::string const &agent, result_type &result) const;

	void detect(const result_type &headers, result_type &result) const;

private:
	typedef details::detector_impl<result_type> impl_type;
	shared_ptr<impl_type> impl_;
};

} // namespace

#endif // UATRAITS_DETECTOR_HPP_INCLUDED

