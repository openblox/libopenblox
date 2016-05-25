dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([libopenblox], [0.1.1], [johnmh@openblox.org])
AC_PROG_CXX

AC_CANONICAL_SYSTEM

AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([1.10 foreign])

PKG_CHECK_MODULES([LCURL], [libcurl])
PKG_CHECK_MODULES([LLUA], [lua])

AC_CHECK_LIB(curl, curl_easy_init,, AC_MSG_ERROR([curl_easy_init was not found in libcurl (curl not installed?)]))
AC_CHECK_LIB(lua, lua_newstate,, AC_MSG_ERROR([lua_newstate was not found in liblua (lua not installed?)]))

LT_INIT

AC_CONFIG_FILES(Makefile
                libopenblox/Makefile
                include/Makefile)
AC_OUTPUT
