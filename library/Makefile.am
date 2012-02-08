lib_LTLIBRARIES = libuatraits.la

libuatraits_la_SOURCES = detector.cpp detector_helper.cpp \
	error.cpp pcre_utils.cpp \
	type_traits.cpp xml_utils.cpp

AM_CXXFLAGS = -pthread
AM_CPPFLAGS = -I${top_srcdir}/include -I${topc_srcdir}/config \
	@XML_CPPFLAGS@ @PCRE_CFLAGS@
AM_LDFLAGS = @XML_LIBS@ @PCRE_LIBS@
