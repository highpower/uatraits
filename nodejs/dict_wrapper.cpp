#include "acsetup.hpp"

#include "dict_wrapper.hpp"

#include "uatraits/details/functors.hpp"

namespace uatraits {

namespace nodejs {

dict_assigner::dict_assigner(dict_wrapper *wrapper, std::string const &name) :
	wrapper_(wrapper), name_(name)
{
}

dict_assigner&
dict_assigner::operator = (std::string const &value) {
	wrapper_->set(name_, value);
	return *this;
}

dict_wrapper::dict_wrapper(v8::Local<v8::Object> &dict) :
	dict_(dict)
{
}

dict_assigner
dict_wrapper::operator [] (std::string const &name) {
	return dict_assigner(this, name);
}

void
dict_wrapper::set(std::string const &name, std::string const &value) {
	dict_->Set(v8::String::NewSymbol(name.c_str()), v8::String::New(value.c_str()));
}

std::string
dict_wrapper::get(std::string const &name) const {
	v8::String::Utf8Value value(v8::Local<v8::String>::Cast(dict_->Get(v8::String::NewSymbol(name.c_str()))));

	if (*value != NULL) {
		return *value;
	}

	return "";
}

bool
dict_wrapper::has(std::string const &name) const {
	v8::Local<v8::String> value(v8::Local<v8::String>::Cast(dict_->Get(v8::String::NewSymbol(name.c_str()))));

	return !value.IsEmpty();
}

} // namespace nodejs

} // namespace uatraits

