
FLAGS=-g -c
SOURCES:=$(wildcard *.cpp)
HEADERS:=$(wildcard *.h)
INC:=-I $(shell $(ROOTSYS)/bin/root-config --cflags)
LIBS:=$(shell $(ROOTSYS)/bin/root-config --ldflags --glibs) -lRooFit -lRooFitCore -lMinuit
OUTPUT:=libTreeProcessor.a

libTreeProcessor.a : ${SOURCES} ${HEADERS}
	g++ ${SOURCES} ${INC} ${LIBS} ${FLAGS}
	ar rcs ${OUTPUT} *.o
	rm -f *.o

clear :
	rm -f libTreeProcessor.a *.o