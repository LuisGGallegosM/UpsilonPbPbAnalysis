#ifndef MAIN
#define MAIN

#include <iostream>
#include <vector>
#include "TTree.h"
#include "TFile.h"
#include "TString.h"
#include "TH1.h"
#include "TCanvas.h"
#include "RooHist.h"

#include "OniaSkimmer.h"
#include "OniaCutter.h"
#include "JetSkimmer.h"
#include "JetCutter.h"
#include "OniaMassFit.h"
#include "TextDrawer.h"

#define ONIATTREENAME ("onia_skimmed")
#define JETTTREENAME ("jet_skimmed")

//skimming
void Skim(const char* filename,const char* outputfilename);

//fitting
void massfit(const char* filename, const char* outfilename, const kineCutParam* kineCut);

//drawing
void Drawing(const char* filename, const kineCutParam* kineCut);

#if defined(__CLING__)
#include "OniaSkimmer.cpp"
#include "OniaCutter.cpp"
#include "JetSkimmer.cpp"
#include "JetCutter.cpp"
#include "Skimming.cpp"
#include "OniaMassFit.cpp"
#include "Fitter.cpp"
#include "TextDrawer.cpp"
#include "Drawing.cpp"
#endif

#endif