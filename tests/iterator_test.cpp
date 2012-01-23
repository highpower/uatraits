#include "acsetup.hpp"

#include <list>
#include <vector>
#include <algorithm>

#include <boost/mpl/list.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

#include "mock_iterator.hpp"

namespace uatraits { namespace tests {

template <typename Iter, typename Tag>
struct test_accessor {
    static void do_access(Iter begin, Iter end);
};

template <typename Iter>
struct test_accessor<Iter, std::random_access_iterator_tag> {
    static void do_access(Iter begin, Iter end);
};

template <typename Iter, typename Tag> void
test_accessor<Iter, Tag>::do_access(Iter begin, Iter end) {
    (void) begin; (void) end;
}

template <typename Iter> void
test_accessor<Iter, std::random_access_iterator_tag>::do_access(Iter begin, Iter end) {
    std::sort(begin, end);
    std::random_shuffle(begin, end);
}

template <typename Iter> void
do_test_access(Iter begin, Iter end) {
    test_accessor<Iter, typename std::iterator_traits<Iter>::iterator_category>::do_access(begin, end);
}

BOOST_AUTO_TEST_SUITE(iterator_test)

typedef boost::mpl::list<std::list<int>, std::vector<int> > container_type_list;

BOOST_AUTO_TEST_CASE_TEMPLATE(test_access, Sequence, container_type_list) {

	typedef mock_iterator<typename Sequence::iterator> iterator_type;
	
	Sequence values;
	for (std::size_t i = 0; i < 10000; ++i) {
		values.push_back(i);
	}
	
	std::size_t count = 0;
	for (iterator_type i(values.begin()), end(values.end()); i != end; ++i, ++count) {
	 	BOOST_CHECK_EQUAL(count, *i);
	}
	do_test_access(iterator_type(values.begin()), iterator_type(values.end()));
	
	typedef std::reverse_iterator<iterator_type> reversed_iterator_type;
	std::size_t result = std::count(reversed_iterator_type(values.end()), reversed_iterator_type(values.begin()), 1000);
	BOOST_CHECK_EQUAL(1, result);
}

BOOST_AUTO_TEST_SUITE_END();

}} // namespaces
