#ifndef YIELDFITTER
#define YIELDFITTER

#include"RooRealVar.h"
#include"RooFormulaVar.h"
#include"RooDataHist.h"
#include"TH1.h"

#include"../../Utils/Serialization/Serialization.h"

class YieldFitter
{
    RooRealVar A;
    RooRealVar B;
    RooRealVar C;
    RooRealVar D;
    RooRealVar pt;
    RooFormulaVar fitFunc;
    TH1F* data;
    RooDataHist dataHist;
    RooDataHist dataHist2;

    public:
    YieldFitter(TH1F* yieldRatio);
    RooAbsReal* fit();
    RooRealVar* getVar() { return &pt;}
    RooDataHist* getHist() {return &dataHist2;}
    ParameterGroup getParams() const;
};

#endif