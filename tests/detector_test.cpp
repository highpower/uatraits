#include "acsetup.hpp"

#include <map>
#include <queue>
#include <string>
#include <fstream>
#include <iostream>

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/test/unit_test.hpp>

#include "uatraits/error.hpp"
#include "uatraits/detector.hpp"
#include "uatraits/details/resource.hpp"
#include "uatraits/details/xml_elems.hpp"
#include "uatraits/details/xml_utils.hpp"

namespace uatraits { namespace tests {

template <typename T>
class threaded_queue {
public:
	threaded_queue();
	virtual ~threaded_queue();

	bool pop(T &v);
	void push(T const &t);
	void stop();
	
private:
	threaded_queue(threaded_queue const &);
	threaded_queue& operator = (threaded_queue const &);
	
private:
	bool stopped_;
	std::queue<T> q_;
	boost::mutex mutex_;
	boost::condition cond_;
};

template <typename T> inline
threaded_queue<T>::threaded_queue() :
	stopped_(false)
{
}

template <typename T> inline
threaded_queue<T>::~threaded_queue() {
}

template <typename T> inline bool
threaded_queue<T>::pop(T &t) {
	boost::mutex::scoped_lock sl(mutex_);
	while (!stopped_ && q_.empty()) {
		cond_.wait(sl);
	}
	if (!stopped_) {
		T temp = q_.front();
		q_.pop();
		std::swap(temp, t);
		return true;
	}
	return false;
}

template <typename T> inline void
threaded_queue<T>::push(T const &t) {
	boost::mutex::scoped_lock sl(mutex_);
	if (!stopped_) {
		q_.push(t);
		cond_.notify_all();
	}
}

template <typename T> inline void
threaded_queue<T>::stop() {
	boost::mutex::scoped_lock sl(mutex_);
	stopped_ = true;
	cond_.notify_all();
}

void
test_detection_with(xmlNodePtr node, detector const &det) {
	
	using namespace details;

	char const *agent = 0;
	std::map<std::string, std::string> props;
	for (xmlNodePtr current = node->children ; current; current = current->next) {
		if (XML_ELEMENT_NODE != current->type) {
			continue;
		}
		if (xmlStrncasecmp(current->name, (xmlChar const*) "question", sizeof("question")) == 0) {
			agent = xml_node_text(current);
		}
		else if (xmlStrncasecmp(current->name, (xmlChar const*) "answer", sizeof("answer")) == 0) {
			xml_elems elems(current, "field");
			for (xml_elems::iterator i = elems.begin(), end = elems.end(); i != end; ++i) {
				char const *name = xml_attr_text(*i, "name");
				char const *value = xml_node_text(*i);
				props.insert(std::pair<std::string, std::string>(name, value));
			}
		}
	}
	std::map<std::string, std::string> result = det.detect(agent);
	BOOST_CHECK_EQUAL(result.size(), props.size());
	for (std::map<std::string, std::string>::const_iterator i = result.begin(), end = result.end(); i != end; ++i) {
		BOOST_CHECK_EQUAL(i->second, props[i->first]);
	}
}

BOOST_AUTO_TEST_SUITE(detector_test)

BOOST_AUTO_TEST_CASE(test_detection) {
	
	using namespace details;
	detector det(getenv("DATAFILE"));
	
	resource<xmlDocPtr, xml_doc_traits> doc(xmlParseFile("cover.xml"));
	xml_throw_unless(doc);
	xmlNodePtr root = xmlDocGetRootElement(doc.get());
	if (xmlStrncasecmp(root->name, (xmlChar const*) "tests", sizeof("tests")) != 0) {
		throw error("bad test data");
	}
	xml_elems elems(root, "test");
	for (xml_elems::iterator i = elems.begin(), end = elems.end(); i != end; ++i) {
		test_detection_with(*i, det);
	}
}

BOOST_AUTO_TEST_CASE(test_multithreading) {

	using namespace details;
	detector det(getenv("DATAFILE"));
	
	resource<xmlDocPtr, xml_doc_traits> doc(xmlParseFile("cover.xml"));
	xml_throw_unless(doc);
	xmlNodePtr root = xmlDocGetRootElement(doc.get());
	if (xmlStrncasecmp(root->name, (xmlChar const*) "tests", sizeof("tests")) != 0) {
		throw error("bad test data");
	}
	xml_elems elems(root, "test");
	for (xml_elems::iterator i = elems.begin(), end = elems.end(); i != end; ++i) {
	}
	
}

BOOST_AUTO_TEST_SUITE_END();

}} // namespaces
