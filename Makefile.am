SUBDIRS = include library utils data

if HAVE_PYTHON
SUBDIRS += python
endif

if HAVE_BOOST_UNIT_TEST
SUBDIRS += tests
endif

ACLOCAL_AMFLAGS = -I config

EXTRA_DIST = autogen.sh AUTHORS ChangeLog COPYING INSTALL NEWS README \
	config/ax_boost_header.m4 config/ax_boost_lib.m4 \
	config/ax_boost_prefix.m4 config/ax_boost_python.m4 \
	config/ax_boost_unit_test.m4 config/ax_check_compiler_flags.m4 \
	config/ax_prefix_config_h.m4 config/ax_python_devel.m4 config/libxml2.m4

all-local:
if HAVE_PERL
	cd perl && perl Makefile.PL && echo "check : test" >> Makefile && make && cd -
endif

check-local:
if HAVE_PERL
	cd perl && make check && cd -
endif

clean-local:
if HAVE_PERL
	cd perl && make clean && perl Makefile.PL && echo "check : test" >> Makefile && cd -
endif

distclean-local:
if HAVE_PERL
	cd perl && make clean && perl Makefile.PL && echo "check : test" >> Makefile && cd -
endif

install-data-local:
if HAVE_PERL
	cd perl && make install && cd -
endif

uninstall-local:
if HAVE_PERL
	cd perl && make uninstall && cd -
endif
