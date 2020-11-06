#!/bin/bash
set -v
FLAGS="-g"
SOURCE="*.cpp ../Utils/serialize.cpp"
INC="-I $(${ROOTSYS}/bin/root-config --cflags)"
LIBS="$(${ROOTSYS}/bin/root-config --ldflags --glibs) -lRooFit -lRooFitCore -lMinuit"
OUTPUT="-o fit"

g++ $FLAGS $INC $SOURCE $LIBS $OUTPUT