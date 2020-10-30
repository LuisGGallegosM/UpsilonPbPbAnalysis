#ifndef MAIN
#define MAIN

#include <iostream>
#include <vector>
#include "TSystem.h"
#include "TTree.h"
#include "TFile.h"
#include "TString.h"
#include "TH1.h"
#include "TCanvas.h"
#include "RooHist.h"

#include "Skimming/OniaSkimmer.h"
#include "Skimming/OniaCutter.h"
#include "Skimming/JetSkimmer.h"
#include "Skimming/JetCutter.h"
#include "Fitting/OniaMassFit.h"
#include "Drawing/TextDrawer.h"
#include "Params.h"

#define ONIATTREENAME ("onia_skimmed")
#define JETTTREENAME ("jet_skimmed")

void Skimming(const char* filename,const char* outputfilename, const cutParams* kineCut);

void Fitting(const char* filename, const char* outfilename, const fitConfig* config);

void Drawing(const char* filename,const char* drawfilename, const drawConfig* config);

#if defined(__CLING__)
#include "Skimming/OniaSkimmer.cpp"
#include "Skimming/OniaCutter.cpp"
#include "Skimming/JetSkimmer.cpp"
#include "Skimming/JetCutter.cpp"
#include "Skimming.cpp"
#include "Fitting/OniaMassFit.cpp"
#include "Fitting.cpp"
#include "Drawing/TextDrawer.cpp"
#include "Drawing.cpp"
#include "Params.cpp"
#endif

#endif