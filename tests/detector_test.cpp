#include "acsetup.hpp"

#include <map>
#include <queue>
#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/function.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/thread.hpp>
#include <boost/test/unit_test.hpp>

#include "uatraits/error.hpp"
#include "uatraits/detector.hpp"
#include "uatraits/details/resource.hpp"
#include "uatraits/details/xml_elems.hpp"
#include "uatraits/details/xml_utils.hpp"

#define THREADSAFE_CHECK(m) if (!(m)) throw std::logic_error(#m)
#define THREADSAFE_CHECK_EQUAL(a, b) if ((a) != (b)) throw std::logic_error(#a " != " #b)

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
	std::queue<T> q_;
	boost::mutex mutex_;
	boost::condition cond_;
};

template <typename T> inline
threaded_queue<T>::threaded_queue()
{
}

template <typename T> inline
threaded_queue<T>::~threaded_queue() {
}

template <typename T> inline bool
threaded_queue<T>::pop(T &t) {
	boost::mutex::scoped_lock sl(mutex_);
	while (q_.empty()) {
		cond_.wait(sl);
	}
	T temp = q_.front();
	if (!temp) {
		return false;
	}
	q_.pop();
	std::swap(temp, t);
	return true;
}

template <typename T> inline void
threaded_queue<T>::push(T const &t) {
	boost::mutex::scoped_lock sl(mutex_);
	q_.push(t);
	cond_.notify_all();
}

template <typename T> inline void
threaded_queue<T>::stop() {
	push(0);
}

static boost::mutex global_mutex;

static void
test_detection_with(xmlNodePtr node, detector const &det) {

	using namespace details;

	char const *agent = 0;
	detector::result_type props;
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
				THREADSAFE_CHECK(name);
				char const *value = xml_node_text(*i);
				THREADSAFE_CHECK(value);
				props.insert(std::pair<std::string, std::string>(name, value));
			}
		}
	}
	THREADSAFE_CHECK(agent);
	detector::result_type result;
	det.detect(agent, result);
	THREADSAFE_CHECK_EQUAL(result.size(), props.size());
	for (detector::result_type::const_iterator i = props.begin(), end = props.end(); i != end; ++i) {
		detector::result_type::const_iterator it = result.find(i->first);
		THREADSAFE_CHECK_EQUAL(i->second, it->second);
	}
}

static bool thread_failure = false;

void
run_multithreaded_test(threaded_queue<xmlNodePtr> &queue, detector const &det, std::size_t &count) {
	try {
		count = 0;
		xmlNodePtr node = 0;
		while (queue.pop(node)) {
			test_detection_with(node, det);
			++count;
		}
	}
	catch (std::exception const &e) {
		std::cerr << "error in thread: " << e.what() << std::endl;
		thread_failure = true;
	}
}

BOOST_AUTO_TEST_SUITE(detector_test)

BOOST_AUTO_TEST_CASE(test_detection) {
	
	using namespace details;
	detector det(getenv("DATAFILE"));

	BOOST_CHECK(!det.date().empty());
	
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
	threaded_queue<xmlNodePtr> queue;

	enum { THREADS = 10 };

	boost::thread_group grp;
	std::size_t counts[THREADS] = { 0 };
	for (std::size_t i = 0; i < THREADS; ++i) {
		boost::function<void()> f(boost::bind(&run_multithreaded_test, boost::ref(queue), boost::cref(det), boost::ref(counts[i])));
		grp.create_thread(f);
	}
	
	resource<xmlDocPtr, xml_doc_traits> doc(xmlParseFile("cover.xml"));
	xml_throw_unless(doc);
	xmlNodePtr root = xmlDocGetRootElement(doc.get());
	if (xmlStrncasecmp(root->name, (xmlChar const*) "tests", sizeof("tests")) != 0) {
		throw error("bad test data");
	}

	std::size_t pushed = 0, processed = 0;
	xml_elems elems(root, "test");
	for (xml_elems::iterator i = elems.begin(), end = elems.end(); i != end; ++i) {
		queue.push(*i);
		++pushed;
	}
	queue.stop();
	grp.join_all();
	BOOST_CHECK(!thread_failure);

	for (std::size_t j = 0; j < THREADS; ++j) {
		processed += counts[j];
	}
	BOOST_CHECK_EQUAL(pushed, processed);
}

BOOST_AUTO_TEST_SUITE_END();

}} // namespaces
