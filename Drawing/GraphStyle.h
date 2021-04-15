#ifndef GRAPHSTYLE
#define GRAPHSTYLE

#include "TCanvas.h"

#include "RooPlot.h"
#include "../OniaBase/Params/Params.h"

void setGraphStyle(RooPlot* plot, const ParameterGroup* config, float topValue ,float bottomValue , bool isLog);
void setPullStyle(RooPlot* pullPlot, const ParameterGroup* config);

TPad* getStyledGraphPad(bool isLog);
TCanvas* getStyledCanvas();
TPad* getStyledPullPad();

#if defined(__CLING__)
#include "GraphStyle.cpp"
#endif

#endif