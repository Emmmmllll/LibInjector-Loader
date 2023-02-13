LIBSRC = libloader.cpp dllinjector.cpp process.cpp
CLISRC = dllinjectorCli.cpp
LIBNAME = libmemory
CLINAME = dllij

LINUXCOMPILER = g++
WINDOWSCOMPILER = 
MACOSCOMPILER = 

linux:
	cd linux/src; ${LINUXCOMPILER} -o ../lib${LIBNAME}.so ${LIBSRC} -shared -fpic -Wall -Werror
	cd linux/src; ${LINUXCOMPILER} -o ../${CLINAME} ${CLISRC} -llibmemory -Wall -Werror -L..

windows:

macos:


.PHONY: linux windows macos