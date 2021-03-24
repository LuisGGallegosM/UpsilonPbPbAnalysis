
FLAGS=$(shell ../HelperScripts/compilerFlags.sh ) -o
SOURCES:=$(shell find . -name  "*.cpp")
HEADERS:=$(shell find . -name  "*.h")
STATICLIBS:=../Utils/libUtils.a ../OniaBase/libOniaBase.a ../Drawing/libDrawing.a
INC:=-I $(shell $(ROOTSYS)/bin/root-config --cflags)
LIBS:=$(shell $(ROOTSYS)/bin/root-config --ldflags --glibs) -lRooFit -lRooFitCore -lMinuit
OUTPUT:=fit

fit : ${SOURCES} ${HEADERS} ${STATICLIBS}
	g++ ${SOURCES} ${STATICLIBS} ${INC} ${LIBS} ${FLAGS} ${OUTPUT}
	objdump $(shell ../HelperScripts/objdumpFlags.sh) ${OUTPUT} > ${OUTPUT}.s

../Utils/libUtils.a :
	$(MAKE) -C ../Utils

../OniaBase/libOniaBase.a :
	$(MAKE) -C ../OniaBase

../Drawing/libDrawing.a :
	$(MAKE) -C ../Drawing

clear :
	rm -f fit *.o *.s