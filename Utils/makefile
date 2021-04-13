
FLAGS:=$(shell ../HelperScripts/compilerFlags.sh ) -c
TESTFLAGS:=$(shell ../HelperScripts/compilerFlags.sh ) -o
SOURCES:=$(shell find ./Helpers ./Serialization ./Tester -name  "*.cpp")
HEADERS:=$(shell find ./Helpers ./Serialization -name  "*.h")
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

test : Test/Test.cpp ${SOURCES} ${HEADERS}
	g++ Test/Test.cpp ${SOURCES} ${INC} ${LIBS} ${TESTFLAGS} test