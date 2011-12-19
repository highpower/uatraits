pyexec_LTLIBRARIES = uatraits.la

uatraits_la_SOURCES = module.cpp

uatraits_la_LDFLAGS = -module -avoid-version @PYTHON_LDFLAGS@ \
	@BOOST_LDFLAGS@ @BOOST_PYTHON_LDFLAGS@ 
uatraits_la_LIBADD = ${top_srcdir}/library/libuatraits.la

AM_CXXFLAGS = -pthread
AM_CPPFLAGS = -I${top_srcdir}/include -I${top_srcdir}/config \
	@PYTHON_CPPFLAGS@ @BOOST_CPPFLAGS@
