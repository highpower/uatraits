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

#ifndef UATRAITS_DETAILS_REGEX_DEFINITION_HPP_INCLUDED
#define UATRAITS_DETAILS_REGEX_DEFINITION_HPP_INCLUDED

#include <pcre.h>
#include <cstdlib>

#include "uatraits/config.hpp"
#include "uatraits/details/resource.hpp"
#include "uatraits/details/definition.hpp"
#include "uatraits/details/pcre_utils.hpp"
#include "uatraits/details/string_utils.hpp"

namespace uatraits { namespace details {

template <typename Traits>
class regex_definition : public definition<Traits> {

public:
	regex_definition(char const *name, char const *pattern, char const *result);
	virtual void process(char const *begin, char const *end, Traits &traits) const;

private:
	regex_definition(regex_definition const &);
	regex_definition& operator = (regex_definition const &);
	std::size_t find_all_replaces(std::string const &value);
	
	using definition<Traits>::name;
	typedef std::pair<std::string::size_type, std::string::size_type> replace_data;

private:
	std::string result_;
	resource<pcre*, pcre_traits> regex_;
	resource<pcre_extra*, pcre_extra_traits> extra_;
	std::list<replace_data> replaces_;
};

template <typename Traits> inline 
regex_definition<Traits>::regex_definition(char const *name, char const *pattern, char const *result) :
	definition<Traits>(name), result_(result), regex_(), extra_()
{
	int error_offset = 0;
	char const *error_ptr = 0;
	regex_.reset(pcre_compile(pattern, 0, &error_ptr, &error_offset, 0));
	if (!regex_) {
		throw error("%s at %d of %s", error_ptr, error_offset, pattern);
	}
	extra_.reset(pcre_study(regex_.get(), PCRE_STUDY_JIT_COMPILE, &error_ptr));
	if (!extra_) {
		throw error("%s in %s", &error_ptr, pattern);
	}
	int max = -1;
	int res = pcre_fullinfo(regex_.get(), extra_.get(), PCRE_INFO_CAPTURECOUNT, &max);
	if (0 != res || -1 == max) {
		throw error("can not get capture count from %s", pattern);
	}
	std::size_t replace_count = find_all_replaces(result_);
	if (replace_count > static_cast<std::size_t>(max)) {
	 	throw error("definition intended to replace more items (%llu) than it could capture in %s (%llu)", 
	 	    static_cast<unsigned long long>(replace_count), pattern, static_cast<unsigned long long>(max));
	}
}

template <typename Traits> inline void
regex_definition<Traits>::process(char const *begin, char const *end, Traits &traits) const {
	
	int match[replaces_.size() * 3];
	int result = pcre_exec(regex_.get(), extra_.get(), begin, end - begin, 0, 0, match, replaces_.size());
	if (PCRE_ERROR_NOMATCH == result) {
		return;
	}
	else if (result < 0) {
		throw error("error while regex matching: %d", result);
	}
}

template <typename Traits> inline std::size_t
regex_definition<Traits>::find_all_replaces(std::string const &value) {
	is_equal<char> dollar('$');
	is_numeric<char> numeric_matcher;
	std::string::const_iterator i = value.begin(), begin = value.begin(), end = value.end();
	while (i != end) {
		i = next_matched(i, end, dollar);
		std::string::const_iterator pos = next_not_matched(i + 1, end, numeric_matcher);
		if (std::distance(i, pos) > 1) {
		    replaces_.push_back(std::make_pair(static_cast<std::string::size_type>(i - begin), static_cast<std::string::size_type>(pos - i)));
		}
		i = pos;
	}
	return replaces_.size();
}

}} // namespaces

#endif // UATRAITS_DETAILS_REGEX_DEFINITION_HPP_INCLUDED

