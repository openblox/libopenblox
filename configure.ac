#Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([libopenblox], [0.1.1], [johnmh@openblox.org])
AC_CONFIG_HEADERS([include/config.h include/oblibconfig.h])
AC_PROG_CXX
PKG_PROG_PKG_CONFIG
PKG_INSTALLDIR
AC_LIBTOOL_WIN32_DLL

AC_CANONICAL_TARGET
AC_CANONICAL_SYSTEM

AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([1.10 foreign subdir-objects])

syslua="lua"

OPENBLOX_PWD=$(pwd)
AC_SUBST(OPENBLOX_PWD)

#Options
AC_ARG_WITH(irrlicht, AS_HELP_STRING([--without-irrlicht], [Compile without Irrlicht support.]))
AC_ARG_WITH(freetype, AS_HELP_STRING([--without-freetype], [Compile without freetype support.]))
AC_ARG_WITH(SDL2, AS_HELP_STRING([--without-SDL2], [Compile without SDL2 support.]))
AC_ARG_WITH(fontconfig, AS_HELP_STRING([--without-fontconfig], [Compile without fontconfig support.]))
AC_ARG_WITH(pugixml, AS_HELP_STRING([--wihout-pugixml], [Compile without pugixml.]))
AC_ARG_WITH(bullet, AS_HELP_STRING([--without-bullet], [Compile without Bullet support.]))
AC_ARG_WITH(curl, AS_HELP_STRING([--without-curl], [Compile without curl support.]))
AC_ARG_WITH(enet, AS_HELP_STRING([--without-enet], [Compile without enet support.]))
AC_ARG_WITH(uuid, AS_HELP_STRING([--without-libuuid], [Compile without libuuid support.]))
AC_ARG_WITH(lua, AS_HELP_STRING([--with-lua=LUA_VER], [Compile with a specific system Lua.]), [syslua="$withval"])

#Dependency checks
AC_DEFINE_UNQUOTED(OPENBLOX_VERSION, [0.1.1], [Current OpenBlox version, for use without PACKAGE_ definitions])
AC_MSG_WARN([Warning]);

AS_IF([test "x$syslua" != "xlua"],
      [LLUA_CFLAGS="-I$oldincludedir/$syslua"] [LLUA_LIBS="-l$syslua -lm"] [LDFLAGS="$LDFLAGS -lm"])

AX_PKG_CHECK_MODULES([LLUA], [lua], [])

AC_CHECK_LIB($syslua, lua_newstate,, AC_MSG_ERROR([lua_newstate was not found in lib$syslua (lua not installed?)]))

AS_IF([test "x$with_irrlicht" != "xno"],
      [AC_CHECK_LIB(Irrlicht, createDevice, [have_irrlicht=yes], AC_MSG_ERROR([createDevice was not found in libIrrlicht (Irrlicht not installed?)]))],
      [have_irrlicht=no])

AS_IF([test "x$have_irrlicht" = "xyes"],
      [[LIRRLICHT_LIBS="-lIrrlicht"] AC_DEFINE_UNQUOTED(HAVE_IRRLICHT, 1, [Define to 1 if you have the `Irrlicht' library (-lIrrlicht).])],
      [LIRRLICHT_LIBS=""])

AC_SUBST(LIRRLICHT_LIBS)

AS_IF([test "x$with_SDL2" != "xno"],
      [AX_PKG_CHECK_MODULES([LSDL2], [sdl2], [], [AC_DEFINE_UNQUOTED(HAVE_SDL2, 1, [Define to 1 if you have the `SDL2' library (-lSDL2).])]) LSDL2_IMAGE_LIBS="-lSDL2_image"],
      [have_sdl2=no LSDL2_IMAGE_LIBS=""])

AS_IF([test "x$with_freetype" != "xno"],
      [AX_PKG_CHECK_MODULES([LFREETYPE], [freetype2], [], [AC_DEFINE_UNQUOTED(HAVE_FREETYPE, 1, [Define to 1 if you have the `freetype' library (-lfreetype).])])],
      [have_freetype=no])

AS_IF([test "x$with_fontconfig" != "xno"],
      [AX_PKG_CHECK_MODULES([LFONTCONFIG], [fontconfig], [], [AC_DEFINE_UNQUOTED(HAVE_FONTCONFIG, 1, [Define to 1 if you have the `fontconfig' library (-lfontconfig).])])],
      [have_fontconfig=no])

AS_IF([test "x$with_pugixml" != "xno"],
      [[LPUGIXML_LIBS="-lpugixml"] AC_DEFINE_UNQUOTED(HAVE_PUGIXML, 1, [Define to 1 if you have the `pugixml' library (-lpugixml).])],
      [[LPUGIXML_LIBS=""]])

AC_SUBST(LPUGIXML_LIBS)

AS_IF([test "x$with_bullet" != "xno"],
      [AX_PKG_CHECK_MODULES([LBULLET], [bullet], [], [AC_DEFINE_UNQUOTED(HAVE_BULLET, 1, [Define to 1 if you have the `bullet' library (-lbullet).])])],
      [have_bullet=no])

AS_IF([test "x$with_curl" != "xno"],
      [AX_PKG_CHECK_MODULES([LCURL], [libcurl], [], [AC_DEFINE_UNQUOTED(HAVE_CURL, 1, [Define to 1 if you have the `curl' library (-lcurl).])])],
      [have_curl=no])

AS_IF([test "x$with_enet" != "xno"],
      [AX_PKG_CHECK_MODULES([LENET], [libenet], [], [AC_DEFINE_UNQUOTED(HAVE_ENET, 1, [Define to 1 if you have the `enet' library (-lenet).])])],
      [have_enet=no])

AS_IF([test "x$with_uuid" != "xno"],
      [AX_PKG_CHECK_MODULES([LUUID], [uuid], [], [AC_DEFINE_UNQUOTED(HAVE_UUID, 1, [Define to 1 if you have the `uuid' library (-luuid).])])],
      [have_uuid=no])

AS_IF([test "x$target_os" == "xmingw32"],
      [LGL_LIB="-lopengl32"],
      [LGL_LIB="-lGL"])

AC_SUBST(LGL_LIB)
AC_SUBST(LSDL2_IMAGE_LIBS)

LT_INIT([win32-dll])

AC_CONFIG_FILES(Makefile
                src/Makefile
                include/Makefile
		libopenblox-devel.pc
                libopenblox.pc)

AC_OUTPUT
