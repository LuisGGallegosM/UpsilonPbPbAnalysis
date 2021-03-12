
FLAGS=$(shell ../HelperScripts/compilerFlags.sh ) -c
SOURCES:=$(shell find . -name  "*.cpp")
HEADERS:=$(shell find . -name  "*.h")
STATICLIBS:=../Utils/libUtils.a ../TreeProcessor/libTreeProcessor.a
INC:=-I $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOTLIBS:=$(shell $(ROOTSYS)/bin/root-config --ldflags --glibs) -lRooFit -lRooFitCore -lMinuit
OUTPUT:=libOniaBase.a

libOniaBase.a : ${SOURCES} ${HEADERS} ${STATICLIBS}
	g++ ${SOURCES} ${INC} ${ROOTLIBS} ${LIBS} ${FLAGS}
	ar xv ../Utils/libUtils.a
	ar xv ../TreeProcessor/libTreeProcessor.a
	ar rcs ${OUTPUT} *.o
	rm -f *.o

../TreeProcessor/libTreeProcessor.a :
	$(MAKE) -C ../TreeProcessor

../Utils/libUtils.a :
	$(MAKE) -C ../Utils

clear :
	rm -f libOniaBase.a *.o