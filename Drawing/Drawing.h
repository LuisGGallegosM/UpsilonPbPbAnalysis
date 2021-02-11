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
#include "RooAbsPdf.h"

#include "TLegend.h"
#include "TLatex.h"
#include "TLine.h"
#include "TStyle.h"

#include "GraphStyle.h"
#include "TextDrawer.h"
#include "../Utils/DrawParams.h"
#include "../Utils/utils.h"
#include "DrawingCmp.h"

#define FITFUNCNAME "fitFunc"
#define DATASETNAME "dataset"

void Drawing(const char* filename,const char* drawfilename);

#endif