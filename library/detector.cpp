#include "acsetup.hpp"
#include "uatraits/detector.hpp"

#include "uatraits/details/resource.hpp"
#include "uatraits/details/detector_impl.hpp"

namespace uatraits { 

detector::detector(char const *name)
{
	using namespace details;
	resource<xmlDocPtr, xml_doc_traits> doc(xmlReadFile(name, 0, XML_PARSE_NOENT));
	xml_throw_unless(doc);
	impl_.reset(new impl_type(doc.get()));
}

detector::detector(char const *name, char const *profiles)
{
	using namespace details;
	resource<xmlDocPtr, xml_doc_traits> doc(xmlReadFile(name, 0, XML_PARSE_NOENT));
	xml_throw_unless(doc);

	resource<xmlDocPtr, xml_doc_traits> profiles_doc(xmlReadFile(profiles, 0, XML_PARSE_NOENT));
	xml_throw_unless(profiles_doc);

	impl_.reset(new impl_type(doc.get(), profiles_doc.get()));
}

detector::~detector() {
}

std::string const&
detector::date() const {
	return impl_->date();
}

detector::result_type
detector::detect(char const *agent) const {
	result_type result;
	impl_->detect(agent, agent + std::char_traits<char>::length(agent), result);
	return result;
}

detector::result_type
detector::detect(char const *agent, size_t len) const {
	result_type result;
	impl_->detect(agent, agent + len, result);
	return result;
}

detector::result_type
detector::detect(std::string const &agent) const {
	result_type result;
	impl_->detect(agent.c_str(), agent.c_str() + agent.size(), result);
	return result;
}

void
detector::detect(char const *agent, result_type &result) const {
	result_type res;
	impl_->detect(agent, agent + std::char_traits<char>::length(agent), res);
	res.swap(result);
}

void
detector::detect(char const *agent, size_t len, result_type &result) const {
	result_type res;
	impl_->detect(agent, agent + len, res);
	res.swap(result);
}

void
detector::detect(std::string const &agent, result_type &result) const {
	result_type res;
	impl_->detect(agent.c_str(), agent.c_str() + agent.size(), res);
	res.swap(result);
}

void
detector::detect(const result_type &headers, result_type &result) const {
	result_type res;
	impl_->detect(headers, res);
	res.swap(result);
}

} // namespace
