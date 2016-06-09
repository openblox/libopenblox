#Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([libopenblox], [0.1.1], [johnmh@openblox.org])
AC_CONFIG_HEADERS([include/config.h])
AC_PROG_CXX

AC_CANONICAL_SYSTEM

AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([1.10 foreign])

#Options
AC_ARG_WITH(irrlicht, AS_HELP_STRING([--without-irrlicht], [Compile without Irrlicht support.]))

#Dependency checks
PKG_CHECK_MODULES([LCURL], [libcurl])
PKG_CHECK_MODULES([LLUA], [lua])

AS_IF([test "x$with_irrlicht" != "xno"],
      [AC_CHECK_LIB(Irrlicht, createDevice, [have_irrlicht=yes], AC_MSG_ERROR([createDevice was not found in libIrrlicht (Irrlicht not installed?)]))],
	  [have_irrlicht=no])

AS_IF([test "x$have_irrlicht" = "xyes"],
      [LIRRLICHT_CFLAGS="-lIrrlicht"] [AC_DEFINE_UNQUOTED(HAVE_IRRLICHT, 1, [Define to 1 if you have the `Irrlicht' library (-lIrrlicht).],
	  [LIRRLICHT_CFLAGS=""])])

AC_CHECK_LIB(curl, curl_easy_init,, AC_MSG_ERROR([curl_easy_init was not found in libcurl (curl not installed?)]))
AC_CHECK_LIB(lua, lua_newstate,, AC_MSG_ERROR([lua_newstate was not found in liblua (lua not installed?)]))

LT_INIT

AC_CONFIG_FILES(Makefile
                src/Makefile
                include/Makefile)
AC_OUTPUT
