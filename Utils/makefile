
FLAGS=$(shell ../HelperScripts/compilerFlags.sh ) -c
SOURCES:=$(shell find . -name  "*.cpp")
HEADERS:=$(shell find . -name  "*.h")
INC:=-I $(shell $(ROOTSYS)/bin/root-config --cflags)
OUTPUT:=libUtils

libUtils.a : ${SOURCES} ${HEADERS}
	g++ ${SOURCES} ${INC} ${FLAGS}
	ar rcs ${OUTPUT}.a *.o
	objdump $(shell ../HelperScripts/objdumpFlags.sh) ${OUTPUT}.a > ${OUTPUT}.s
	rm -f *.o

clear :
	rm -f libUtils.a *.o *.s