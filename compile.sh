g++ -g -I `${ROOTSYS}/bin/root-config --cflags` -I *.cpp "Drawing/"*.cpp "Fitting/"*.cpp "Skimming/"*.cpp `${ROOTSYS}/bin/root-config --ldflags --glibs` -lRooFit -lRooFitCore -lMinuit
