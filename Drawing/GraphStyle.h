#ifndef GRAPHSTYLE
#define GRAPHSTYLE

#include "TCanvas.h"

#include "RooPlot.h"
#include "../Utils/Params.h"

void setGraphStyle(RooPlot* plot, const drawConfig* config, float topValue ,float bottomValue , bool isLog);
void setPullStyle(RooPlot* pullPlot, const drawConfig* config);

TPad* getStyledGraphPad(bool isLog);
TCanvas* getStyledCanvas();
TPad* getStyledPullPad();

#if defined(__CLING__)
#include "GraphStyle.cpp"
#endif

#endif