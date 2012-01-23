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

#ifndef UATRAITS_DETAILS_REGEX_DEFINITION_HPP_INCLUDED
#define UATRAITS_DETAILS_REGEX_DEFINITION_HPP_INCLUDED

#include <vector>
#include <cstdlib>
#include <iostream>

#include "uatraits/config.hpp"
#include "uatraits/details/definition.hpp"
#include "uatraits/details/pcre_utils.hpp"
#include "uatraits/details/regex_utils.hpp"

namespace uatraits { namespace details {

template <typename Traits>
class regex_definition : public definition<Traits> {

public:
	regex_definition(char const *name, char const *xpath, char const *pattern, char const *result);
	virtual ~regex_definition();

	virtual void dump(std::ostream &out) const;
	virtual bool detect(char const *begin, char const *end, Traits &traits) const;

private:
	regex_definition(regex_definition const &);
	regex_definition& operator = (regex_definition const &);
	
	using definition<Traits>::name;
	using definition<Traits>::xpath;

private:
	std::string replace_pattern_;
	std::list<regex_data> replaces_;
	std::pair<pcre*, pcre_extra*> regex_;
};

template <typename Traits> inline 
regex_definition<Traits>::regex_definition(char const *name, char const *xpath, char const *pattern, char const *replace_pattern) :
	definition<Traits>(name, xpath), replace_pattern_(replace_pattern), regex_(0, 0)
{
	int max = -1;
	regex_ = pcre_compile_regex(pattern);
	int res = pcre_fullinfo(regex_.first, regex_.second, PCRE_INFO_CAPTURECOUNT, &max);
	if (0 != res || -1 == max) {
		throw error("can not get capture count from %s: %d", pattern, res);
	}
	find_replaces(replace_pattern_, replaces_);
	std::size_t replace_count = replaces_.size();
	if (replace_count > static_cast<std::size_t>(max)) {
	 	throw error("definition intended to replace more items (%llu) than it could capture in %s (%llu)", 
	 	    static_cast<unsigned long long>(replace_count), pattern, static_cast<unsigned long long>(max));
	}
}

template <typename Traits> inline
regex_definition<Traits>::~regex_definition() {
    pcre_free_regex(regex_);
}

template <typename Traits> inline void
regex_definition<Traits>::dump(std::ostream &out) const {
    out << "regex definition at [" << xpath() << "] triggered: setting " << name() << "=" << replace_pattern_ << " being substituted during detection" << std::endl;
}

template <typename Traits> inline bool
regex_definition<Traits>::detect(char const *begin, char const *end, Traits &traits) const {
	
	std::vector<int> match((replaces_.size() + 1) * 3, 0);
	int result = pcre_exec(regex_.first, regex_.second, begin, end - begin, 0, 0, &match[0], match.size());
	if (PCRE_ERROR_NOMATCH == result) {
		return false;
	}
	else if (result < 0) {
		throw error("error while regex matching: %d", result);
	}
	if (static_cast<std::size_t>(result) != replaces_.size() + 1) {
	    throw error("error while regex matching: captured %d while desired %d", result - 1, replaces_.size());
	}
	std::string temp(replace_pattern_);
	for (std::list<regex_data>::const_reverse_iterator ri = replaces_.rbegin(), rend = replaces_.rend(); ri != rend; ++ri) {
    	temp.replace(temp.begin() + ri->begin, temp.begin() + ri->end, begin + match[2 * ri->index], begin + match[2 * ri->index + 1]);
	}
	traits[name()] = temp;
	return true;
}

}} // namespaces

#endif // UATRAITS_DETAILS_REGEX_DEFINITION_HPP_INCLUDED

