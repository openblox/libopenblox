PRO_Lib = OpenBlox.pro

ifeq ($(OS),Windows_NT)
	QMAKE = C:\Qt\5.4\mingw491_32\bin\qmake.exe
else
	QMAKE = qmake
endif

all: lib

clean:
	rm -fr QtMakefile QtMakefile.Debug QtMakefile.Release debug release openblox
QtMakefile_Lib:
	$(QMAKE) -o QtMakefile $(PRO_Lib) CONFIG+=debug_and_release

lib-debug:	QtMakefile_Lib
	$(MAKE) -j 2 -f QtMakefile debug

lib-release:	QtMakefile_Lib
	$(MAKE) -j 2 -f QtMakefile release

lib:	QtMakefile_Lib
	$(MAKE) -j 2 -f QtMakefile

docs: doxygen

.PHONY: all clean lib-debug lib-release lib QtMakefile_Lib docs
