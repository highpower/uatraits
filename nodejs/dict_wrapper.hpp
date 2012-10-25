#ifndef UATRAITS_NODEJS_DICT_WRAPPER_HPP_INCLUDED
#define UATRAITS_NODEJS_DICT_WRAPPER_HPP_INCLUDED

#include <string>

#include <v8.h>

namespace uatraits {

namespace nodejs {

class dict_wrapper;

class dict_assigner { 
  
public:
	dict_assigner(dict_wrapper *wrapper, std::string const &name);
	dict_assigner& operator = (std::string const &value);
  
private:
	dict_wrapper *wrapper_;
	std::string const &name_;
};
  
class dict_wrapper { 
  
public:
	dict_wrapper(v8::Local<v8::Object> &dict);
	dict_assigner operator [] (std::string const &name);
	void set(std::string const &name, std::string const &value);
	std::string get(std::string const &name) const;
	bool has(std::string const &name) const;
  
private:
	v8::Local<v8::Object> &dict_;
};

} // namespace nodejs

} // namespace uatraits

#endif // UATRAITS_NODEJS_DICT_WRAPPER_HPP_INCLUDED

