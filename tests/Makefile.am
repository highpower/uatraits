check_PROGRAMS = uatraits-tests

noinst_HEADERS = mock_iterator.hpp mock_shared.hpp mock_template.hpp \
	test_utils.hpp

uatraits_tests_SOURCES = main.cpp mock_shared.cpp \
	detector_test.cpp functors_test.cpp integer_test.cpp iterator_test.cpp \
	range_test.cpp regex_utils_test.cpp shared_ptr_test.cpp string_test.cpp \
	static_assert_test.cpp type_list_test.cpp type_traits_test.cpp

uatraits_tests_LDADD = ${top_srcdir}/library/libuatraits.la
	
AM_CXXFLAGS = -pthread
AM_CPPFLAGS = -I${top_srcdir}/include -I${top_srcdir}/config @BOOST_CPPFLAGS@
AM_LDFLAGS = @BOOST_LDFLAGS@ @BOOST_UNIT_TEST_LDFLAGS@

TESTS = uatraits-tests
TESTS_ENVIRONMENT = DATAFILE=${top_srcdir}/data/browser.xml

noinst_DATA = msie.txt