#include "acsetup.hpp"

#include <map>
#include <string>
#include <fstream>
#include <iostream>

#include <boost/test/unit_test.hpp>

#include "uatraits/error.hpp"
#include "uatraits/detector.hpp"
#include "uatraits/details/resource.hpp"
#include "uatraits/details/xml_elems.hpp"
#include "uatraits/details/xml_utils.hpp"

namespace uatraits { namespace tests {

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
	if (result.size() != props.size()) {
		BOOST_CHECK_EQUAL(result.size(), props.size());
		std::cout << "-----------------------------------------------------" << std::endl;
		std::cout << "result size: " << result.size() << ", pros size: " << props.size() << std::endl;
		for (std::map<std::string, std::string>::const_iterator i = result.begin(), end = result.end(); i != end; ++i) {
			std::cout << i->first << " = " << i->second << std::endl;
		}
		std::cout << "but: " << std::endl;
		for (std::map<std::string, std::string>::const_iterator i = props.begin(), end = props.end(); i != end; ++i) {
			std::cout << i->first << " = " << i->second << std::endl;
		}
	}
	else {
		for (std::map<std::string, std::string>::const_iterator i = result.begin(), end = result.end(); i != end; ++i) {
			std::map<std::string, std::string>::const_iterator p = props.find(i->first);
			BOOST_REQUIRE(p != props.end());
			BOOST_CHECK_EQUAL(i->second, p->second);
		}
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

BOOST_AUTO_TEST_SUITE_END();

}} // namespaces
