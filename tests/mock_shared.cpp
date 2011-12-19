#include "acsetup.hpp"
#include "mock_shared.hpp"

namespace uatraits { namespace tests {

std::set<mock_shared*> 
mock_shared::objects_;

mock_shared::mock_shared()
{
	objects_.insert(this);
}

mock_shared::~mock_shared() {
	objects_.erase(this);
}

bool
mock_shared::is_any_alive() {
	return !objects_.empty();
}

bool
mock_shared::is_alive(mock_shared *object) {
	return objects_.find(object) != objects_.end();
}

mock_shared_derived::mock_shared_derived()
{
}

mock_shared_derived::~mock_shared_derived() {
}

}} // namespaces
