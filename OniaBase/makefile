
FLAGS=$(shell ../HelperScripts/compilerFlags.sh ) -c
SOURCES:=$(shell find . -name  "*.cpp")
HEADERS:=$(shell find . -name  "*.h")
STATICLIBS:=../Utils/libUtils.a
INC:=-I $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOTLIBS:=$(shell $(ROOTSYS)/bin/root-config --ldflags --glibs) -lRooFit -lRooFitCore -lMinuit
OUTPUT:=libOniaBase

libOniaBase.a : ${SOURCES} ${HEADERS} ${STATICLIBS}
	g++ ${SOURCES} ${INC} ${ROOTLIBS} ${LIBS} ${FLAGS}
	ar xv ../Utils/libUtils.a
	ar rcs ${OUTPUT}.a *.o
	objdump $(shell ../HelperScripts/objdumpFlags.sh) ${OUTPUT}.a > ${OUTPUT}.s
	rm -f *.o

../Utils/libUtils.a :
	$(MAKE) -C ../Utils

clear :
	rm -f libOniaBase.a *.o *.s