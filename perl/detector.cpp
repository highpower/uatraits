#include "acsetup.hpp"
#include "detector.hpp"

#include "uatraits/details/resource.hpp"
#include "uatraits/details/xml_utils.hpp"
#include "uatraits/details/detector_impl.hpp"

#include "hash_utils.hpp"

namespace uatraits { namespace perl {

class hash_wrapper;

class hash_assigner {

public:
	hash_assigner(hash_wrapper *hash, std::string const &name);
	hash_assigner& operator = (std::string const &value);
	
private:
	hash_wrapper *hash_;
	std::string const &name_;	
};

class hash_wrapper {

public:
	hash_wrapper(void *hv);
	hash_assigner operator [] (std::string const &name);
	void set(std::string const &name, std::string const &value);

private:
	void *hash_value_;
};

detector::detector(char const *file) :
	impl_()
{
	using namespace details;
	resource<xmlDocPtr, xml_doc_traits> doc(xmlReadFile(file, 0, XML_PARSE_NOENT));
	xml_throw_unless(doc);
	impl_.reset(new impl_type(doc.get()));
}

detector::~detector() {
}

void
detector::detect(char const *begin, char const *end, void *hv) const {
	hash_wrapper wrapper(hv);
	impl_->detect(begin, end, wrapper);
}

hash_assigner::hash_assigner(hash_wrapper *hash, std::string const &name) :
	hash_(hash), name_(name)
{
}

hash_assigner&
hash_assigner::operator = (std::string const &value) {
	hash_->set(name_, value);
	return *this;
}

hash_wrapper::hash_wrapper(void *hv) :
	hash_value_(hv)
{
}

hash_assigner
hash_wrapper::operator [] (std::string const &name) {
	return hash_assigner(this, name);
}

void
hash_wrapper::set(std::string const &name, std::string const &value) {
	hash_value_set(hash_value_, name.c_str(), name.size(), value.c_str(), value.size());
}

}} // namespaces
