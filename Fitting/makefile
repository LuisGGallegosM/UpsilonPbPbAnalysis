
FLAGS=$(shell ../HelperScripts/compilerFlags.sh ) -o
SOURCES:=$(shell find . -name  "*.cpp" ! -path "./Test/*")
TESTSOURCES:=$(shell find . -name  "*.cpp" ! -name "Fitting.cpp")
HEADERS:=$(shell find . -name  "*.h" ! -path "./Test/*")
STATICLIBS:=../Utils/libUtils.a ../OniaBase/libOniaBase.a ../Drawing/libDrawing.a
INC:=-I $(shell $(ROOTSYS)/bin/root-config --cflags)
LIBS:=$(shell $(ROOTSYS)/bin/root-config --ldflags --glibs) -lRooFit -lRooFitCore -lMinuit
OUTPUT:=fit

all: fit test
	./test

fit : ${SOURCES} ${HEADERS} ${STATICLIBS}
	g++ ${SOURCES} ${STATICLIBS} ${INC} ${LIBS} ${FLAGS} ${OUTPUT}
	objdump $(shell ../HelperScripts/objdumpFlags.sh) ${OUTPUT} > ${OUTPUT}.s

../Utils/libUtils.a :
	$(MAKE) -C ../Utils

../OniaBase/libOniaBase.a :
	$(MAKE) -C ../OniaBase

../Drawing/libDrawing.a :
	$(MAKE) -C ../Drawing

test : ${TESTSOURCES} ${HEADERS} ${STATICLIBS}
	g++ ${TESTSOURCES} ${STATICLIBS} ${INC} ${LIBS} ${FLAGS} test

clear :
	rm -f fit test *.o *.s