
FLAGS=$(shell ../HelperScripts/compilerFlags.sh ) -c
SOURCES:=$(wildcard *.cpp)
HEADERS:=$(wildcard *.h)
INC:=-I $(shell $(ROOTSYS)/bin/root-config --cflags)
OUTPUT:=libDrawing

draw : ${SOURCES} ${HEADERS} ${STATICLIBS}
	g++ ${SOURCES} ${INC} ${FLAGS}
	ar rcs ${OUTPUT}.a *.o
	objdump $(shell ../HelperScripts/objdumpFlags.sh) ${OUTPUT}.a > ${OUTPUT}.s
	rm -f *.o

clear :
	rm -f ${OUTPUT}.a *.o *.s