AC_DEFUN([_AX_CHECK_PCRE_IN],
[
	ax_prefix=$1
	AC_MSG_CHECKING([whether pcre is installed in $1])
	ax_have_pcre_in="yes"
	if test ! -x "$ax_prefix/bin/pcre-config"; then
		ax_have_pcre_in="no"
	fi
	AC_MSG_RESULT([$ax_have_pcre_in])
	if test "f$ax_have_pcre_in" = "fyes"; then
		ifelse([$2], , :, [$2])
	else
		ifelse([$3], , :, [$3])
	fi
])

AC_DEFUN([AX_CHECK_PCRE],
[
	ax_pcre_prefix=""
	AC_ARG_WITH([pcre-prefix], 
		AS_HELP_STRING(--with-pcre-prefix, specifies pcre installation),
	[
		if test "f$withval" != "f"; then
			ax_pcre_prefix="$withval"
		fi
	])
	if test "f$ax_pcre_prefix" != "f"; then
		_AX_CHECK_PCRE_IN([$ax_pcre_prefix], [], [ax_pcre_prefix=""])
	else
		for i in /usr/local /usr; do
			_AX_CHECK_PCRE_IN([$i], [ax_pcre_prefix="$i"], [])
		done;
	fi
	
	ax_have_pcre="no"
	if test "f$ax_pcre_prefix" != "f"; then
		ax_pcre_libs=`$ax_pcre_prefix/bin/pcre-config --libs`
		ax_pcre_cflags=`$ax_pcre_prefix/bin/pcre-config --cflags`
		ax_check_pcre_stored_ldflags="$LDFLAGS"
		ax_check_pcre_stored_cppflags="$CPPFLAGS"
		LDFLAGS="$ax_check_pcre_stored_ldflags $ax_pcre_libs"
		CPPFLAGS="$ax_check_pcre_stored_cppflags $ax_pcre_cflags"
		AC_LANG_SAVE()
		AC_LANG([C])
		AC_MSG_CHECKING([link with pcre])
		AC_LINK_IFELSE(
			[ AC_LANG_PROGRAM([#include <pcre.h>], [void *f = &pcre_compile]) ],
			[ AC_MSG_RESULT(yes); ax_have_pcre="yes" ], 
			[ AC_MSG_RESULT(no) ])
		AC_LANG_RESTORE()
		CPPFLAGS="$ax_check_pcre_stored_cppflags"
		LDFLAGS="$ax_check_pcre_stored_ldflags"
	fi
	if test "f$ax_have_pcre" != "fno"; then
		AC_SUBST([PCRE_LIBS], ["$ax_pcre_libs"])
		AC_SUBST([PCRE_CFLAGS], ["$ax_pcre_cflags"])
		ifelse([$1], , :, [$1])
	else
		AC_SUBST([PCRE_LIBS], [])
		AC_SUBST([PCRE_CFLAGS], [])
		ifelse([$2], , :, [$2])
	fi
])
