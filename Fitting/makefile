
FLAGS=-g -o
SOURCES:=$(wildcard *.cpp)
HEADERS:=$(wildcard *.h)
STATICLIBS:=../Utils/libUtils.a ../TreeProcessor/libTreeProcessor.a
INC:=-I $(shell $(ROOTSYS)/bin/root-config --cflags)
LIBS:=$(shell $(ROOTSYS)/bin/root-config --ldflags --glibs) -lRooFit -lRooFitCore -lMinuit
OUTPUT:=fit

fit : ${SOURCES} ${HEADERS} ${STATICLIBS}
	g++ ${SOURCES} ${STATICLIBS} ${INC} ${LIBS} ${FLAGS} ${OUTPUT}

../TreeProcessor/libTreeProcessor.a :
	$(MAKE) -C ../TreeProcessor

../Utils/libUtils.a :
	$(MAKE) -C ../Utils

clear :
	rm fit