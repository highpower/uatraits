#include "acsetup.hpp"
#include "detector.hpp"

#include <map>
#include <string>

#include "uatraits/details/resource.hpp"
#include "uatraits/details/xml_utils.hpp"
#include "uatraits/details/detector_impl.hpp"

namespace uatraits { namespace perl {

class hash;

class hash_assigner {

public:
	hash_assigner(hash *parent, std::string const &name);
	hash_assigner& operator = (std::string const &value);

private:
	hash *parent_;
	std::string const name_;
};

class hash : public enumeration<detector::item> {

public:
	hash();
	virtual ~hash();
	
	bool empty() const;
	detector::item next() const;

	hash_assigner operator [] (std::string const &name);
	void set(std::string const &name, std::string const &value);
	
private:
	hash(hash const &);
	hash& operator = (hash const &);
	typedef std::map<std::string, std::string> map_type;
	
	void reset();

private:
	map_type map_;
	map_type::const_iterator begin_, end_;
};

detector::detector(char const *name) :
	impl_()
{
	using namespace details;
	resource<xmlDocPtr, xml_doc_traits> doc(xmlParseFile(name));
	xml_throw_unless(doc);
	impl_.reset(new impl_type(doc.get()));
}

detector::~detector() {
}

enumeration<detector::item>::pointer
detector::detect(char const *begin, char const *end) const {
	shared_ptr<hash> result(new hash());
	impl_->detect(begin, end, *result);
	return result.cast<enumeration<detector::item>::type>();
}

hash_assigner::hash_assigner(hash *parent, std::string const &name) :
	parent_(parent), name_(name)
{
}

hash_assigner&
hash_assigner::operator = (std::string const &value) {
	parent_->set(name_, value);
}

bool
hash::empty() const {
	return map_.empty() ? true : begin_ == end_;
}

detector::item
hash::next() const {
	
}

hash_assigner
hash::operator [] (std::string const &name) {
	return hash_assigner(this, name);
}

void
hash::set(std::string const &name, std::string const &value) {
	map_[name] = value;
	reset();
}

void
hash::reset() {
	end_ = map_.end();
	begin_ = map_.begin();
}

}} // namespaces
