
FLAGS:=$(shell ../HelperScripts/compilerFlags.sh ) -c
TESTFLAGS:=$(shell ../HelperScripts/compilerFlags.sh ) -o
SOURCES:=$(shell find . -name  "*.cpp" ! -path "./Test/*")
HEADERS:=$(shell find . -name  "*.h" ! -path "./Test/*")
INC:=-I $(shell $(ROOTSYS)/bin/root-config --cflags)
LIBS:=$(shell $(ROOTSYS)/bin/root-config --ldflags --glibs) -lRooFit -lRooFitCore -lMinuit
OUTPUT:=libUtils

all : libUtils.a test

libUtils.a : ${SOURCES} ${HEADERS}
	g++ ${SOURCES} ${INC} ${FLAGS}
	ar rcs ${OUTPUT}.a *.o
	objdump $(shell ../HelperScripts/objdumpFlags.sh) ${OUTPUT}.a > ${OUTPUT}.s
	rm -f *.o

clear :
	rm -f libUtils.a *.o *.s
	rm test

test : Test/Test.cpp libUtils.a
	g++ Test/Test.cpp libUtils.a ${INC} ${LIBS} ${TESTFLAGS} test