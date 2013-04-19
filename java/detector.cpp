#include "detector.h"

uatraits::java::Detector::Detector(char const *file)
{
	detector = new uatraits::detector(file);
}

uatraits::java::Detector::~Detector()
{
	delete detector;
}

std::map<std::string, std::string> uatraits::java::Detector::detect(char const *str) const throw(std::runtime_error) {
	std::map<std::string, std::string> result;
	if (!str) {
		detector->detect("", result);
	} else {
		detector->detect(str, result);
	}
	return result;
}
