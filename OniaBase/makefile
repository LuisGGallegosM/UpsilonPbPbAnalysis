
FLAGS=$(shell ../HelperScripts/compilerFlags.sh ) -c
TESTFLAGS:=$(shell ../HelperScripts/compilerFlags.sh ) -o
SOURCES:=$(shell find . -name  "*.cpp" ! -path "./Test/*")
HEADERS:=$(shell find . -name  "*.h" ! -path "./Test/*")
STATICLIBS:=../Utils/libUtils.a
INC:=-I $(shell $(ROOTSYS)/bin/root-config --cflags)
LIBS:=$(shell $(ROOTSYS)/bin/root-config --ldflags --glibs) -lRooFit -lRooFitCore -lMinuit
OUTPUT:=libOniaBase

all : libOniaBase.a test
	./test

libOniaBase.a : ${SOURCES} ${HEADERS} ${STATICLIBS}
	g++ ${SOURCES} ${INC} ${LIBS} ${FLAGS}
	ar xv ../Utils/libUtils.a
	ar rcs ${OUTPUT}.a *.o
	objdump $(shell ../HelperScripts/objdumpFlags.sh) ${OUTPUT}.a > ${OUTPUT}.s
	rm -f *.o

../Utils/libUtils.a :
	$(MAKE) -C ../Utils

clear :
	rm -f libOniaBase.a *.o *.s
	rm -f test

test : Test/Test.cpp libOniaBase.a
	g++ Test/Test.cpp libOniaBase.a ${INC} ${LIBS} ${TESTFLAGS} test