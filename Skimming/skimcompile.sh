#!/bin/bash
set -v
FLAGS="-g"
SOURCE="*.cpp ../Utils/*.cpp"
INC="-I $(${ROOTSYS}/bin/root-config --cflags)"
LIBS="$(${ROOTSYS}/bin/root-config --ldflags --glibs) -lRooFit -lRooFitCore -lMinuit"
OUTPUT="-o skim"

g++ $FLAGS $INC $SOURCE $LIBS $OUTPUT