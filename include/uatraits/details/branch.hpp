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

#ifndef UATRAITS_DETAILS_BRANCH_HPP_INCLUDED
#define UATRAITS_DETAILS_BRANCH_HPP_INCLUDED

#include <list>
#include <iostream>
#include <algorithm>

#include "uatraits/config.hpp"
#include "uatraits/shared.hpp"
#include "uatraits/shared_ptr.hpp"
#include "uatraits/details/definition.hpp"
#include "uatraits/details/functors.hpp"
#include "uatraits/details/pcre_utils.hpp"

namespace uatraits { namespace details {

template <typename Traits>
class branch : public shared {

public:
	branch(char const *xpath);
	virtual ~branch();

	typedef branch<Traits> type;
	typedef definition<Traits> definition_type;

	bool is_common() const;
	void set_common(bool value);
	
	bool is_default() const;
	void set_default(bool value);
	
	void add_match(char const *pattern);
	void add_child(shared_ptr<type> const &child);
	void add_definition(shared_ptr<definition_type> const &value);
	void add_regex_match(char const *pattern);
	void trigger(char const *begin, char const *end, Traits &traits) const;
	
	virtual bool matched(char const *begin, char const *end) const;

private:
	branch(branch const &);
	branch& operator = (branch const &);

	typedef shared_ptr<type> pointer;
	typedef shared_ptr<definition_type> definition_pointer;
	typedef std::pair<pcre*, pcre_extra*> regex_data;

private:
	std::string xpath_;
	bool common_, default_;
	std::list<pointer> children_;
	std::list<definition_pointer> definitions_;
	std::list<regex_data> regex_matches_;
	std::list<std::string> string_matches_;
};

template <typename Traits> 
class root_branch : public branch<Traits> {

public:
	root_branch();
	virtual bool matched(char const *begin, char const *end) const;
};

template <typename Traits> inline
branch<Traits>::branch(char const *xpath) :
    xpath_(xpath), common_(false), default_(false)
{
}

template <typename Traits> inline 
branch<Traits>::~branch() {
	for (std::list<regex_data>::iterator i = regex_matches_.begin(), end = regex_matches_.end(); i != end; ++i) {
		pcre_free_regex(*i);
	}
}

template <typename Traits> inline bool
branch<Traits>::is_common() const {
    return common_;
}

template <typename Traits> inline void
branch<Traits>::set_common(bool value) {
    common_ = value;
}

template <typename Traits> inline bool
branch<Traits>::is_default() const {
    return default_;
}

template <typename Traits> inline void
branch<Traits>::set_default(bool value) {
    default_ = value;
}

template <typename Traits> inline void
branch<Traits>::add_match(char const *pattern) {
	string_matches_.push_back(std::string(pattern));
}

template <typename Traits> inline void
branch<Traits>::add_child(shared_ptr<type> const &child) {
	children_.push_back(child);
}

template <typename Traits> inline void
branch<Traits>::add_definition(shared_ptr<definition_type> const &value) {
	definitions_.push_back(value);
}

template <typename Traits> inline void
branch<Traits>::add_regex_match(char const *pattern) {
	regex_matches_.push_back(pcre_compile_regex(pattern));
}

template <typename Traits> inline void
branch<Traits>::trigger(char const *begin, char const *end, Traits &traits) const {
	for (typename std::list<definition_pointer>::const_iterator i = definitions_.begin(), list_end = definitions_.end(); i != list_end; ++i) {
		(*i)->trigger(begin, end, traits);
	}
	bool worked = false;
	pointer default_branch;

	for (typename std::list<pointer>::const_iterator i = children_.begin(), list_end = children_.end(); i != list_end; ++i) {
	    if ((*i)->is_default()) {
	        default_branch = *i;
	    }
	    else if ((*i)->is_common()) {
    		(*i)->trigger(begin, end, traits);
	    } 
	    else if (!worked && (*i)->matched(begin, end)) {
    		worked = true;
	    	(*i)->trigger(begin, end, traits);
	    }
	}
	if (!worked && default_branch) {
	    default_branch->trigger(begin, end, traits);
	}
}

template <typename Traits> inline bool
branch<Traits>::matched(char const *begin, char const *end) const {
	for (std::list<std::string>::const_iterator i = string_matches_.begin(), list_end = string_matches_.end(); i != list_end; ++i) {
		if (std::search(begin, end, i->begin(), i->end(), ci_equal<char>()) != end) {
			return true;
		}
	}
	for (std::list<regex_data>::const_iterator i = regex_matches_.begin(), list_end = regex_matches_.end(); i != list_end; ++i) {
		if (0 == pcre_exec(i->first, i->second, begin, end - begin, 0, 0, 0, 0)) {
		 	return true;
		}
	}
	return false;
}

template <typename Traits> inline
root_branch<Traits>::root_branch() :
    branch<Traits>("")
{
}

template <typename Traits> inline bool
root_branch<Traits>::matched(char const *begin, char const *end) const {
	(void) begin; (void) end;
	return true;
}

}} // namespaces

#endif // UATRAITS_DETAILS_BRANCH_HPP_INCLUDED
