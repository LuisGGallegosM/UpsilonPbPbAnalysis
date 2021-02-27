
FLAGS=-g -c
SOURCES:=$(wildcard *.cpp)
HEADERS:=$(wildcard *.h)
STATICLIBS:=../Utils/libUtils.a ../TreeProcessor/libTreeProcessor.a
INC:=-I $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOTLIBS:=$(shell $(ROOTSYS)/bin/root-config --ldflags --glibs) -lRooFit -lRooFitCore -lMinuit
OUTPUT:=libOniaBase.a

libOniaBase.a : ${SOURCES} ${HEADERS} ${STATICLIBS}
	g++ ${SOURCES} ${INC} ${ROOTLIBS} ${LIBS} ${FLAGS}
	ar xv ../Utils/libUtils.a
	ar xv ../TreeProcessor/libTreeProcessor.a
	ar rcs ${OUTPUT} *.o 
	rm *.o

../TreeProcessor/libTreeProcessor.a :
	$(MAKE) -C ../TreeProcessor

../Utils/libUtils.a :
	$(MAKE) -C ../Utils

clear :
	rm libOniaBase.a