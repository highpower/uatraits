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

#ifndef UATRAITS_DETAILS_COMPLEX_DEFINITION_HPP_INCLUDED
#define UATRAITS_DETAILS_COMPLEX_DEFINITION_HPP_INCLUDED

#include <list>
#include <string>

#include "uatraits/config.hpp"
#include "uatraits/details/definition.hpp"

namespace uatraits { namespace details {

template <typename Traits>
class complex_definition : public definition<Traits> {

public:
	complex_definition(char const *name);
	typedef definition<Traits> definition_type;

	bool has_only_one() const;
	shared_ptr<definition_type> release_child() const;
	
	void add(shared_ptr<definition_type> const &value);
	virtual void process(char const *begin, char const *strend, Traits &traits) const;

private:
	complex_definition(complex_definition const &);
	complex_definition& operator = (complex_definition const &);
	typedef shared_ptr<definition_type> definition_pointer;
	using definition<Traits>::name;

private:
	std::list<definition_pointer> definitions_;
};

template <typename Traits> inline 
complex_definition<Traits>::complex_definition(char const *name) :
	definition<Traits>(name)
{
}

template <typename Traits> inline bool
complex_definition<Traits>::has_only_one() const {
	return (1 == definitions_.size());
}

template <typename Traits> inline shared_ptr<typename complex_definition<Traits>::definition_type>
complex_definition<Traits>::release_child() const {
	return definitions_.front();
}

template <typename Traits> inline void
complex_definition<Traits>::add(shared_ptr<typename complex_definition<Traits>::definition_type> const &value) {
	definitions_.push_back(value);
}

template <typename Traits> inline void
complex_definition<Traits>::process(char const *begin, char const *strend, Traits &traits) const {
	for (typename std::list<definition_pointer>::const_iterator i = definitions_.begin(), end = definitions_.end(); i != end; ++i) {
		(*i)->process(begin, strend, traits);
	}
}

}} // namespaces

#endif // UATRAITS_DETAILS_COMPLEX_DEFINITION_HPP_INCLUDED
