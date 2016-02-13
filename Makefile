.DEFAULT_GOAL := all

PRO = OpenBlox.pro

PACKAGE_TARNAME = libopenblox

QMAKE = /usr/bin/qmake

prefix = /usr
exec_prefix = ${prefix}

includedir = ${prefix}/include
docdir = ${prefix}/share/doc/${PACKAGE_TARNAME}
libdir = ${exec_prefix}/lib
man3dir = ${prefix}/share/man/man3

clean:
	rm -fr QtMakefile QtMakefile.Debug QtMakefile.Release debug release libopenblox.so libopenblox.so.*

QtMakefile:
	$(QMAKE) -o QtMakefile $(PRO) CONFIG+=debug_and_release

debug:	QtMakefile
	$(MAKE) -j 2 -f QtMakefile debug

release:	QtMakefile
	$(MAKE) -j 2 -f QtMakefile release

all:	QtMakefile
	$(MAKE) -j 2 -f QtMakefile

install:
	mkdir -p $DESTDIR$man3dir
	cp -f libopenblox.3.gz $DESTDIR$man3dir
	mkdir -p $DESTDIR$libdir
	cp -f libopenblox.so $DESTDIR$libdir
	mkdir -p $DESTDIR$includedir/libopenblox/ob_lua
	cp -f src/ob_lua/*.h $DESTDIR$includedir/libopenblox/ob_lua
	mkdir -p $DESTDIR$includedir/libopenblox/openblox
	cp -f src/openblox/*.h $includedir/libopenblox/openblox
	mkdir -p $DESTDIR$includedir/libopenblox/openblox/enum
	cp -f src/openblox/enum/*.h $DESTDIR$includedir/libopenblox/openblox/enum
	mkdir -p $DESTDIR$includedir/libopenblox/openblox/instance
	cp -f src/openblox/instance $DESTDIR$includedir/libopenblox/openblox/instance
	mkdir -p $DESTDIR$includedir/libopenblox/openblox/type
	cp -f src/openblox/type $DESTDIR$includedir/libopenblox/openblox/type

docs: doxygen

.PHONY: all clean debug release QtMakefile install docs
