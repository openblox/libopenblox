.DEFAULT_GOAL := all

PRO = OpenBlox.pro

PACKAGE_TARNAME = libopenblox

QMAKE = /usr/bin/qmake

prefix = /usr/local
exec_prefix = ${prefix}

bindir = ${exec_prefix}/bin
datadir = ${prefix}/share/libopenblox
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
	echo ${exec_prefix}/lib

docs: doxygen

.PHONY: all clean debug release QtMakefile install docs
