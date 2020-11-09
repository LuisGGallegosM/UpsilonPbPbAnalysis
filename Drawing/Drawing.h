#ifndef DRAWING
#define DRAWING

#include "RooRealVar.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TFile.h"
#include "RooHist.h"
#include "RooDataSet.h"
#include "RooFitResult.h"
#include "RooPlot.h"

#include "TLegend.h"
#include "TLatex.h"
#include "TLine.h"
#include "TStyle.h"
#include "TH1.h"

#include "TextDrawer.h"
#include "../Utils/Params.h"

#define FITFUNCNAME "fitFunc"
#define DATASETNAME "dataset"

void Drawing(const char* filename,const char* drawfilename);

#if defined(__CLING__)
#include "TextDrawer.cpp"
#include "../Utils/utils.cpp"
#include "../Utils/serialize.cpp"
#endif

#endif