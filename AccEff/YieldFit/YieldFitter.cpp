
#include"YieldFitter.h"
#include"../Common/AccEffAux.h"

#include"RooDataSet.h"

using namespace RooFit;

YieldFitter::YieldFitter(TH1F* yieldRatio):
    A("A","A parameter",0.86,-250000.0,250000.0),
    B("B","B parameter",-0.02,-8000.0,8000.0),
    C("C","C parameter",0.005,-8000.0,8000.0),
    D("D","D parameter",-2.00,-8000.0,8000.0),
    pt("pt","p_{T}",0.0,30.0),
    dataHist("ratioRooHist","ratio of yields rooHist",RooArgList(pt),Import(*yieldRatio,kTRUE)),
    dataHist2("ratioRooHist","ratio of yields rooHist",RooArgList(pt),Import(*yieldRatio,kFALSE)),
    fitFunc(yieldFitFuncName,"fit of ratios","(@1+@2*@0+@3*(@0*@0))/((@0 - @4)^3)",RooArgList(pt,A,B,C,D))
{

}

RooAbsReal* YieldFitter::fit()
{
    RooFitResult* result= fitFunc.chi2FitTo(dataHist,RooFit::Hesse(),RooFit::Timer(),RooFit::Save());
    return &fitFunc;
}