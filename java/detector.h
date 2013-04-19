#ifndef UATRAITS_MODULE_JAVA_H_INCLUDED
#define UATRAITS_MODULE_JAVA_H_INCLUDED

//#include "acsetup.hpp"

#include <map>
#include <string>
#include <stdexcept>
#include "uatraits/detector.hpp"

namespace uatraits { namespace java {

class Detector {
private:
	uatraits::detector * detector;
public:
        Detector(char const *file);
	~Detector();
        std::map<std::string, std::string> detect(char const *str) const throw(std::runtime_error) ;
};
}}
#endif
