bin_PROGRAMS = check-uatraits

check_uatraits_SOURCES = main.cpp
check_uatraits_LDADD = ${top_srcdir}/library/libuatraits.la

AM_CXXFLAGS = -pthread
AM_CPPFLAGS = -I${top_srcdir}/include -I${topc_srcdir}/config @XML_CPPFLAGS@
AM_LDFLAGS = @XML_LIBS@
