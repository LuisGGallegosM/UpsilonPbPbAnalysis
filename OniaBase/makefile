
FLAGS=-g -c
SOURCES:=$(wildcard *.cpp)
HEADERS:=$(wildcard *.h)
STATICLIBS:=../Utils/libUtils.a
INC:=-I $(shell $(ROOTSYS)/bin/root-config --cflags)
LIBS:=$(shell $(ROOTSYS)/bin/root-config --ldflags --glibs) -lRooFit -lRooFitCore -lMinuit
OUTPUT:=libOniaBase.a

libOniaBase.a : ${SOURCES} ${HEADERS} ${STATICLIBS}
	g++ ${SOURCES} ${INC} ${LIBS} ${FLAGS}
	ar xv ${STATICLIBS}
	ar rcs ${OUTPUT} *.o 
	rm *.o

../Utils/libUtils.a :
	$(MAKE) -C ../Utils

clear :
	rm libOniaBase.a