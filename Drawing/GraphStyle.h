#ifndef GRAPHSTYLE
#define GRAPHSTYLE

#include "TCanvas.h"

#include "RooPlot.h"
#include "../Utils/Params.h"

void setGraphStyle(RooPlot* plot,const drawConfig* config);
void setPullStyle(RooPlot* pullPlot);

TPad* getStyledGraphPad();
TCanvas* getStyledCanvas();
TPad* getStyledPullPad();

#if defined(__CLING__)
#include "GraphStyle.cpp"
#endif

#endif