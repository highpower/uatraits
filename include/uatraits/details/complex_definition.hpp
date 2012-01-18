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

#ifndef UATRAITS_DETAILS_COMPLEX_DEFINITION_HPP_INCLUDED
#define UATRAITS_DETAILS_COMPLEX_DEFINITION_HPP_INCLUDED

#include <list>
#include <string>
#include <iosfwd>

#include "uatraits/config.hpp"
#include "uatraits/details/definition.hpp"

namespace uatraits { namespace details {

template <typename Traits>
class complex_definition : public definition<Traits> {

public:
	complex_definition(char const *name, char const *xpath);
	typedef definition<Traits> definition_type;

	bool has_only_one() const;
	shared_ptr<definition_type> release_child() const;
	
	void add(shared_ptr<definition_type> const &value);
	
	virtual void dump(std::ostream &out) const;
	virtual bool detect(char const *begin, char const *strend, Traits &traits) const;
	virtual bool checked_detect(char const *begin, char const *strend, Traits &traits, std::ostream &out) const;

private:
	complex_definition(complex_definition const &);
	complex_definition& operator = (complex_definition const &);
	typedef shared_ptr<definition_type> definition_pointer;
	
	using definition<Traits>::name;
	using definition<Traits>::xpath;

private:
	std::list<definition_pointer> definitions_;
};

template <typename Traits> inline 
complex_definition<Traits>::complex_definition(char const *name, char const *xpath) :
	definition<Traits>(name, xpath)
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
complex_definition<Traits>::dump(std::ostream &out) const {
	out << "complex definition at [" << xpath() << "]" << std::endl;
}

template <typename Traits> inline bool
complex_definition<Traits>::detect(char const *begin, char const *strend, Traits &traits) const {
	bool result = false;
	for (typename std::list<definition_pointer>::const_iterator i = definitions_.begin(), end = definitions_.end(); i != end; ++i) {
		result = (*i)->detect(begin, strend, traits) || result;
	}
	return result;
}

template <typename Traits> inline bool
complex_definition<Traits>::checked_detect(char const *begin, char const *strend, Traits &traits, std::ostream &out) const {
	bool result = false;
	for (typename std::list<definition_pointer>::const_iterator i = definitions_.begin(), end = definitions_.end(); i != end; ++i) {
		bool triggered = (*i)->detect(begin, strend, traits);
		if (triggered) {
			(*i)->dump(out);
		}
		result = result || triggered;
	}
	return result;
}


}} // namespaces

#endif // UATRAITS_DETAILS_COMPLEX_DEFINITION_HPP_INCLUDED
