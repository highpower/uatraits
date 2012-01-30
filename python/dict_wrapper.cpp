#include "acsetup.hpp"
#include "dict_wrapper.hpp"

#include "uatraits/details/functors.hpp"

namespace uatraits { namespace python {

details::range<char const*> const
dict_wrapper::true_token = details::make_range("true");

details::range<char const*> const
dict_wrapper::false_token = details::make_range("false");

dict_assigner::dict_assigner(dict_wrapper *wrapper, std::string const &name) :
	wrapper_(wrapper), name_(name)
{
}

dict_assigner&
dict_assigner::operator = (std::string const &value) {
	wrapper_->set(name_, value);
	return *this;
}

dict_wrapper::dict_wrapper(py::dict &dict) :
	dict_(dict)
{
}

dict_assigner
dict_wrapper::operator [] (std::string const &name) {
	return dict_assigner(this, name);
}

void
dict_wrapper::set(std::string const &name, std::string const &value) {
	if (details::is_ci_equal(value, true_token)) {
		dict_[name] = true;
	}
	else if (details::is_ci_equal(value, false_token)) {
		dict_[name] = false;
	}
	else {
		dict_[name] = value;
	}
}

}} // namespaces
