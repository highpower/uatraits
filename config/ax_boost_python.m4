AC_DEFUN([AX_BOOST_PYTHON],
[
	ax_boost_python_stored_ldflags="$LDFLAGS"
	ax_boost_python_stored_cppflags="$CPPFLAGS"
	ax_boost_python_stored_libs="$LIBS"
	
	AC_REQUIRE([AX_BOOST_PREFIX])
	AC_REQUIRE([AX_PYTHON_DEVEL])
	AX_BOOST_LIB([ax_boost_python_lib], [boost_python])
	if test "f$ax_boost_python_lib" == "f"; then
		AX_BOOST_LIB([ax_boost_python_lib], [boost_python$PYTHON_VERSION])
	fi
	
	CPPFLAGS="$ax_boost_python_stored_cppflags $PYTHON_CPPFLAGS"
	LDFLAGS="$ax_boost_python_stored_ldflags $BOOST_LDFLAGS $PYTHON_LDFLAGS"
	LIBS="-l$ax_boost_python_lib $PYTHON_LIBS"

	ax_have_boost_python="yes"
	AX_BOOST_HEADER([python.hpp], [], [ax_have_boost_python="no"])
	
	AC_MSG_CHECKING([linkage with boost::python])
	AC_LANG_SAVE()
	AC_LANG([C++])
	AC_LINK_IFELSE(
		[ AC_LANG_PROGRAM([#include <boost/python.hpp>], [boost::python::str s;]) ],
		[ AC_MSG_RESULT(yes) ], 
		[ AC_MSG_RESULT(no); ax_have_boost_python="no" ])
	AC_LANG_RESTORE()
	
	CPPFLAGS="$ax_boost_python_stored_cppflags"
	LDFLAGS="$ax_boost_python_stored_ldflags"
	LIBS="$ax_boost_python_stored_libs"
	
	if test "f$ax_have_boost_python" = "fyes"; then
		ifelse([$1], , :, [$1])
		AC_SUBST([BOOST_PYTHON_LDFLAGS], ["-l$ax_boost_python_lib"])
		AC_SUBST([BOOST_PYTHON_LIBS], ["-l$ax_boost_python_lib"])
	else
		ifelse([$2], , :, [$2])
	fi	
])
