#include "acsetup.hpp"
#include "detector.hpp"

#include "uatraits/details/detector_impl.hpp"
#include "uatraits/details/resource.hpp"
#include "uatraits/details/xml_utils.hpp"

#include "hash_utils.hpp"

namespace uatraits {

namespace perl {

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
	std::string get(std::string const &name) const;
	bool has(std::string const &name) const;
	void destroy();

private:
	void *hash_value_;
};

} // namespace perl

namespace details {

template<> inline std::string
detector_impl<perl::hash_wrapper>::find_header(const perl::hash_wrapper &headers, const std::string &header) const {
	return headers.get(header);
}

template <> inline perl::hash_wrapper
detector_impl<perl::hash_wrapper>::detect_by_header(const std::string &header) const {
	perl::hash_wrapper hash(perl::hash_create());

	detect(header.c_str(), header.c_str() + header.size(), hash);

	return hash;
}

template <> inline void
detector_impl<perl::hash_wrapper>::update_attr(perl::hash_wrapper &traits, perl::hash_wrapper &copy,
	const std::string &header) const {
	if (copy.has(header)) {
		std::string str = copy.get(header);

		if (!str.empty()) {
			traits[header] = str;
		}
	}
}

} // namespace details

namespace perl {

detector::detector(char const *file) :
	impl_()
{
	using namespace details;
	resource<xmlDocPtr, xml_doc_traits> doc(xmlReadFile(file, 0, XML_PARSE_NOENT));
	xml_throw_unless(doc);
	impl_.reset(new impl_type(doc.get()));
}

detector::detector(char const *file, char const *profiles) :
	impl_()
{
	using namespace details;
	resource<xmlDocPtr, xml_doc_traits> doc(xmlReadFile(file, 0, XML_PARSE_NOENT));
	xml_throw_unless(doc);
	resource<xmlDocPtr, xml_doc_traits> profiles_doc(xmlReadFile(profiles, 0, XML_PARSE_NOENT));
	xml_throw_unless(profiles_doc);
	impl_.reset(new impl_type(doc.get(), profiles_doc.get()));
}

detector::~detector() {
}

void
detector::detect(char const *begin, char const *end, void *hv) const {
	hash_wrapper wrapper(hv);
	impl_->detect(begin, end, wrapper);
}

void
detector::detect_by_headers(void *headers, void *hv) const {
	hash_wrapper wrapper(hv);
	hash_wrapper headers_wrapper(headers);
	impl_->detect(headers_wrapper, wrapper);
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

std::string
hash_wrapper::get(std::string const &name) const {
	return hash_value_get(hash_value_, name.c_str(), name.size());
}

bool
hash_wrapper::has(std::string const &name) const {
	return hash_has_value(hash_value_, name.c_str(), name.size());
}

void
hash_wrapper::destroy() {
	hash_destroy(hash_value_);
}

}} // namespaces
