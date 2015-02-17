#
# Most of this configure script was ripped from pidgin or the
# pidgin-encryption configure script, thanks guys ;)
#

AC_INIT([pidgin-indicator], [0.9], [philipl@overt.org])
AC_CANONICAL_SYSTEM
AM_CONFIG_HEADER(config.h)

AM_INIT_AUTOMAKE([foreign dist-bzip2])

AC_PREREQ([2.50])

AC_DISABLE_STATIC

dnl This disables check of libtool for these compilers
CXX=no
F77=no
GCJ=no

dnl # Make sure we have pkg-config
PKG_PROG_PKG_CONFIG()

#
# Setup libtool
#
AM_PROG_LIBTOOL
LIBTOOL="$LIBTOOL --silent"

#
# Good o' gettext
#
AC_PROG_INTLTOOL

GETTEXT_PACKAGE=pidgin-indicator
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, ["$GETTEXT_PACKAGE"], [Define the gettext package to be used.])

#ALL_LINGUAS="bn cs de en_AU en_GB es fr gl he hu it ja mk nl no pt pt_BR ru sk sr sr@Latn sv uk zh_CN zh_TW"
ALL_LINGUAS="de hu ru zh_CN"
AM_GLIB_GNU_GETTEXT

#
# Look for the C compiler
#
CFLAGS_save="$CFLAGS"
AC_PROG_CC_STDC
CFLAGS="$CFLAGS_save"

AC_ARG_ENABLE(debug, [  --enable-debug          compile with debugging support],,enable_debug=no)
AC_ARG_ENABLE(deprecated, [  --disable-deprecated    compile without deprecated API usage],,enable_deprecated=yes)

if test "x$enable_debug" = "xyes" ; then
	AC_DEFINE(DEBUG, 1, [Define if debugging is enabled.])
fi

if test "x$enable_deprecated" = no; then
	DEBUG_CFLAGS = "$DEBUG_CFLAGS -DG_DISABLE_DEPRECATED -DGDK_DISABLE_DEPRECATED -DGDK_PIXBUF_DISABLE_DEPRECATED -DGTK_DISABLE_DEPRECATED"
fi
AC_SUBST(DEBUG_CFLAGS)

if test "x$GCC" = "xyes"; then
	CFLAGS="$CFLAGS -Wall -g3"
fi
AC_SUBST(CFLAGS)

#
# Check for pidgin 
#

PKG_CHECK_MODULES(PIDGIN, pidgin purple)

if test x"$prefix" = x"NONE" ; then
	PIDGIN_LIBDIR=`pkg-config --variable=libdir pidgin`
	PIDGIN_DATADIR=`pkg-config --variable=datadir pidgin`
else
	PIDGIN_LIBDIR="$libdir"
	PIDGIN_DATADIR="$datadir"
fi

AC_SUBST(PIDGIN_CFLAGS)
AC_SUBST(PIDGIN_LIBS)
AC_SUBST(PURPLE_CFLAGS)
AC_SUBST(PURPLE_LIBS)
AC_SUBST(PIDGIN_LIBDIR)
AC_SUBST(PIDGIN_DATADIR)

#
# check for gtk and other dependencies
#
PKG_CHECK_MODULES(GLIB, [glib-2.0])
AC_SUBST(GLIB_LIBS)
AC_SUBST(GLIB_CFLAGS)

PKG_CHECK_MODULES(GTK, [gtk+-2.0])
AC_SUBST(GTK_LIBS)
AC_SUBST(GTK_CFLAGS)

PKG_CHECK_MODULES(INDICATOR, [appindicator-0.1])
AC_SUBST(INDICATOR_LIBS)
AC_SUBST(INDICATOR_CFLAGS)

#
# Check if plugins are enabled
#
want_plugins=yes
AM_CONDITIONAL(PLUGINS, test "$want_plugins" = "yes")

#
# Finish up
#
AC_OUTPUT([Makefile
	   po/Makefile.in
	   icons/Makefile
	   src/Makefile
	  ])
