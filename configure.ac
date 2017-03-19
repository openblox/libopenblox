#Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([libopenblox], [0.1.1], [johnmh@openblox.org])
AC_CONFIG_HEADERS([include/config.h include/irrconfig.h])
AC_PROG_CXX

AC_CANONICAL_SYSTEM

AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([1.10 foreign subdir-objects])

syslua="lua"

#Options
AC_ARG_WITH(irrlicht, AS_HELP_STRING([--without-irrlicht], [Compile without Irrlicht support.]))
AC_ARG_WITH(lua, AS_HELP_STRING([--with-lua=LUA_VER], [Compile with a specific system Lua.]), [syslua="$withval"])

#Dependency checks
PKG_CHECK_MODULES([LCURL], [libcurl])

AS_IF([test "x$with_irrlicht" != "xno"],
      [AC_CHECK_LIB(Irrlicht, createDevice, [have_irrlicht=yes], AC_MSG_ERROR([createDevice was not found in libIrrlicht (Irrlicht not installed?)]))],
	  [have_irrlicht=no])

AS_IF([test "x$have_irrlicht" = "xyes"],
      [LIRRLICHT_CFLAGS="-lIrrlicht"] [AC_DEFINE_UNQUOTED(HAVE_IRRLICHT, 1, [Define to 1 if you have the `Irrlicht' library (-lIrrlicht).],
	  [LIRRLICHT_CFLAGS=""])])

AS_IF([test "x$syslua" != "xlua"],
      [LLUA_CFLAGS="-I$oldincludedir/$syslua"] [LLUA_LIBS="-l$syslua -lm"] [LDFLAGS="$LDFLAGS -lm"])

PKG_CHECK_MODULES([LLUA], [lua])

AC_CHECK_LIB(curl, curl_easy_init,, AC_MSG_ERROR([curl_easy_init was not found in libcurl (curl not installed?)]))
AC_CHECK_LIB($syslua, lua_newstate,, AC_MSG_ERROR([lua_newstate was not found in lib$syslua (lua not installed?)]))

LT_INIT

AC_CONFIG_FILES(Makefile
                src/Makefile
                include/Makefile)
AC_OUTPUT
