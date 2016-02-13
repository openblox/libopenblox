AC_INIT(libopenblox, vesion-0.1.1)
AC_PROG_CC
AC_PROG_CXX

#C

#libs
AC_CHECK_LIB(curl, curl_easy_init,, AC_MSG_ERROR([curl_easy_init was not found in libcurl]))
AC_CHECK_LIB(openal, alcOpenDevice,, AC_MSG_ERROR([alcOpenDevice was not found in libopenal]))
AC_CHECK_LIB(alut, alutInit,, AC_MSG_ERROR([alutInit was not found in libalut]))
AC_CHECK_LIB(lua, lua_newstate,, AC_MSG_ERROR([lua_newstate was not found in liblua]))
AC_CHECK_LIB(SDL2, SDL_Init,, AC_MSG_ERROR([curl_easy_init was not found in libcurl]))

#headers
AC_FUNC_ALLOCA
AC_CHECK_HEADERS([float.h limits.h locale.h malloc.h memory.h stdlib.h string.h strings.h unistd.h wchar.h])

#typedefs, structures, compiler characteristics
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_OFF_T
AC_C_RESTRICT
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

#lib funcs
AC_FUNC_MALLOC
AC_FUNC_MKTIME
AC_FUNC_REALLOC
AC_CHECK_FUNCS([gethostbyname gethostname gettimeofday memset mkdir select strcasecmp strchr strerror strncasecmp strpbrk strrchr strspn strstr])

#C++
AC_LANG(C++)

AC_CHECK_HEADERS(iostream)

#qmake
AC_CHECK_PROGS(QMAKE_CHECK, [qmake qmake-qt5], no)
if test x"$QMAKE_CHECK" == x"no" ; then
    AC_MSG_ERROR([Required program ('qmake' or 'qmake-qt5') not found.])
fi

AC_PATH_PROGS(QMAKEVAR, [qmake qmake-qt5])

#makefile
AC_OUTPUT(Makefile)
