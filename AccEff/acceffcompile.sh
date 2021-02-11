#!/bin/bash
set -v
FLAGS="-g"
SOURCE="*.cpp ../Utils/*.cpp ../TreeProcessor/*.cpp"
INC="-I $(${ROOTSYS}/bin/root-config --cflags)"
LIBS="$(${ROOTSYS}/bin/root-config --ldflags --glibs) -lRooFit -lRooFitCore -lMinuit"
OUTPUT="-o acceff"

g++ $FLAGS $INC $SOURCE $LIBS $OUTPUT