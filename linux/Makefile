LIBSRC = libloader.cpp dllinjector.cpp process.cpp
CLISRC = dllinjectorCli.cpp
LIBNAME = libmemory
CLINAME = dllij

LINUXCOMPILER = g++

build:
	cd src; ${LINUXCOMPILER} -o ../lib${LIBNAME}.so ${LIBSRC} -g -shared -fpic -Wall -Werror
	cd src; ${LINUXCOMPILER} -o ../${CLINAME} ${CLISRC} -llibmemory -Wall -Werror -L..

.PHONY: build