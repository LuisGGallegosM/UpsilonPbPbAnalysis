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

#include "../../Drawing/TextDrawer.h"
#include "../../OniaBase/Params/Params.h"
#include "../../Utils/Helpers/Helpers.h"

constexpr char fitFuncName[] = "fitFunc";
constexpr char datasetName[] = "dataset";

#endif