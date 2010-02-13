#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.64])
AC_INIT([mtools], [0.1], [rosek86@gmail.com])
AM_INIT_AUTOMAKE

AC_CONFIG_SRCDIR([src/mtools.cpp])
AC_CONFIG_HEADERS([config.h])

IT_PROG_INTLTOOL([0.41.0])

GETTEXT_PACKAGE=mtools
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE], ["$GETTEXT_PACKAGE"],
                   [The domain to use with gettext])
AM_GLIB_GNU_GETTEXT

MTOOLS_LOCALEDIR=[${datadir}/locale]
AC_SUBST(MTOOLS_LOCALEDIR)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
AC_CHECK_LIB(nfc, nfc_connect,,AC_MSG_ERROR(Install libnfc))
AC_CHECK_LIB(freefare, mifare_classic_connect,,AC_MSG_ERROR(Install libfreefare))

PKG_CHECK_MODULES(DEPS, gtkmm-2.4 >= 2.16)
AC_SUBST(DEPS_CFLAGS)
AC_SUBST(DEPS_LIBS)

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL

# Checks for library functions.

AC_CONFIG_FILES(po/Makefile.in)

AC_OUTPUT( \
	Makefile \
	src/Makefile \
	)
