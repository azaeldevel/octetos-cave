
AC_INIT(octetos-cave, 0.1.0-alpha, azael.devel@gmail.com)
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([1.11])
AM_SILENT_RULES([yes])
AC_PROG_CXX

dnl ***************************************************************************
dnl Internationalization
dnl ***************************************************************************
IT_PROG_INTLTOOL([0.35.0])
GETTEXT_PACKAGE=cave
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [GETTEXT package name])
dnl AM_GLIB_GNU_GETTEXT
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.17])
OCTETOS_CAVE_LOCALEDIR=[${datadir}/locale]
AC_SUBST(OCTETOS_CAVE_LOCALEDIR)

LT_INIT
PKG_CHECK_MODULES(OCTETOS_CAVE_BASE, [octetos-core glibmm-2.4])
s
AC_ARG_ENABLE([os],[AS_HELP_STRING([--enable-os],[Enable configuration for particular OS])])
case $enableval in
	  	archlinux)
			AC_DEFINE([LINUX_ARCH], [], [Enable configuration for Arch-Linux])
		;;
	  	debian) 
			AC_DEFINE([LINUX_DEBIAN], [], [Enable configuration for Debian])
		;;
	  	*)
			AC_MSG_ERROR([unknown option '$enable_os' for --enable-os])
		;;
esac

AC_ARG_ENABLE([tdd],[AS_HELP_STRING([--enable-tdd],[Enable Test-Drive Developing)])])
if test "x$enable_tdd" = "xyes"; then 
	AM_CONDITIONAL(OCTEOTOS_CAVE_ENABLE_TDD, true)
	PKG_CHECK_MODULES(CUNIT, [cunit])
else
	AM_CONDITIONAL(OCTEOTOS_CAVE_ENABLE_TDD, false)
fi
AC_ARG_ENABLE([devel],[AS_HELP_STRING([--enable-devel],[Enable Developer mode)])])
if test "x$enable_devel" = "xyes"; then 
	AM_CONDITIONAL(OCTEOTOS_CAVE_ENABLE_DEV, true)
	PKG_CHECK_MODULES(CUNIT, [cunit])
else
	AM_CONDITIONAL(OCTEOTOS_CAVE_ENABLE_DEV, false)
fi

AM_CONDITIONAL(DATABASE_ENGINE_MARIA,false)
AM_CONDITIONAL(DATABASE_ENGINE_MYSQL,false)
AM_CONDITIONAL(DATABASE_ENGINE_POSTGRESQL,false)
AC_ARG_ENABLE([database],[AS_HELP_STRING([--enable-database],[Enabling sopport for Database])])
case $enableval in
	maria)
		PKG_CHECK_MODULES(OCTETOS_CAVE_MARIA, [octetos-db-maria])
		AM_CONDITIONAL(DATABASE_ENGINE_MARIA,true)
	;;
	postgresql)
		PKG_CHECK_MODULES(OCTETOS_CAVE_PQ, [octetos-db-postgresql])
		AM_CONDITIONAL(DATABASE_ENGINE_POSTGRESQL,true)
	;;
	*)
		AC_MSG_ERROR([unknown option '$enable_database' for --enable-database])
	;;
esac

AC_OUTPUT([
Makefile
src/Makefile
po/Makefile.in
tests/Makefile
])
