
FLAGS=$(shell ../HelperScripts/compilerFlags.sh ) -c
SOURCES:=$(wildcard *.cpp)
HEADERS:=$(wildcard *.h)
INC:=-I $(shell $(ROOTSYS)/bin/root-config --cflags)
LIBS:=$(shell $(ROOTSYS)/bin/root-config --ldflags --glibs) -lRooFit -lRooFitCore -lMinuit
OUTPUT:=libTreeProcessor

libTreeProcessor.a : ${SOURCES} ${HEADERS}
	g++ ${SOURCES} ${INC} ${LIBS} ${FLAGS}
	ar rcs ${OUTPUT}.a *.o
	objdump -d -S -Mintel --no-show-raw-insn ${OUTPUT}.a > ${OUTPUT}.s
	rm -f *.o

clear :
	rm -f libTreeProcessor.a *.o *.s