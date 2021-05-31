
#include"YieldFitter.h"
#include"../Common/AccEffAux.h"

#include"RooDataSet.h"

using namespace RooFit;

YieldFitter::YieldFitter(TH1D* yieldRatio):
    A("A","A parameter",244.0,-1000.0,1000.0),
    B("B","B parameter",17.0,-100.0,100.0),
    C("C","C parameter",23.0,-100.0,100.0),
    D("D","D parameter",-5.00,-100.0,100.0),
    pt("pt","p_{T}",0.0,100.0),
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

ParameterGroup YieldFitter::getParams() const
{
    ParameterGroup g;

    ParameterWrite(g,A,"A");
    ParameterWrite(g,B,"B");
    ParameterWrite(g,C,"C");
    ParameterWrite(g,D,"D");

    return g;
}