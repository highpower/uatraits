pyexec_LTLIBRARIES = uatraits.la

uatraits_la_SOURCES = module.cpp streambuf.cpp

uatraits_la_LDFLAGS = -module -avoid-version @PYTHON_LDFLAGS@ \
	@XML_LIBS@ @PCRE_LIBS@ @BOOST_LDFLAGS@ @BOOST_PYTHON_LDFLAGS@ 
uatraits_la_LIBADD = ${top_srcdir}/library/libuatraits.la

AM_CXXFLAGS = -pthread
AM_CPPFLAGS = -I${top_srcdir}/include -I${top_srcdir}/config \
	@XML_CPPFLAGS@ @PCRE_CFLAGS@ @PYTHON_CPPFLAGS@ @BOOST_CPPFLAGS@

noinst_HEADERS = streambuf.hpp
