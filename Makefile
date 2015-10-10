PRO_Lib = OpenBlox.pro

ifeq ($(OS),Windows_NT)
	QMAKE = C:\Qt\5.4\mingw491_32\bin\qmake.exe
else
	QMAKE = qmake
endif

clean:
	rm -fr QtMakefile QtMakefile.Debug QtMakefile.Release debug release libopenblox.so libopenblox.so.*
QtMakefile_Lib:
	$(QMAKE) -o QtMakefile $(PRO_Lib) CONFIG+=debug_and_release

debug:	QtMakefile_Lib
	$(MAKE) -j 2 -f QtMakefile debug

release:	QtMakefile_Lib
	$(MAKE) -j 2 -f QtMakefile release

all:	QtMakefile_Lib
	$(MAKE) -j 2 -f QtMakefile

docs: doxygen

.PHONY: all clean debug release lib QtMakefile_Lib docs
