#include <iostream>
#include <vector>
#include "TTree.h"
#include "TFile.h"
#include "TString.h"
#include "TH1.h"
#include "TCanvas.h"

#include "OniaSkimmer.h"
#include "OniaCutter.h"
#include "JetSkimmer.h"
#include "JetCutter.h"
#include "OniaMassFit.h"

#define ONIATTREENAME ("onia_skimmed")
#define JETTTREENAME ("jet_skimmed")

void Skim(const char* filename,const char* outputfilename);

#if defined(__CLING__)
#include "OniaSkimmer.cpp"
#include "OniaCutter.cpp"
#include "JetSkimmer.cpp"
#include "JetCutter.cpp"
#include "Skimming.cpp"
#include "OniaMassFit.cpp"
#endif

