

#include "OniaMassFit.h"

//OniaMassFitter member functions.

OniaMassFitter::OniaMassFitter(TTree* tree_,float massLow_,float massHigh_):
    massLow(massLow_), massHigh(massHigh_),tree(tree_),
    coeff1("c1","Crystal ball 1 coeff", 10001.0, 0.0, 10000000.0),
    coeff2("c2","Crystal ball 2 coeff", 10000.0, 0.0, 10000000.0),
    mass("mass","onia mass",massLow,massHigh),
    dataset("dataset","mass dataset",tree_,mass),
    cball1(mass,"1"),
    cball2(mass,"2"),
    dcball("dcb_fit","double crystal ball", RooArgList(*(cball1.getCB()),*(cball2.getCB()) ),RooArgList(coeff1,coeff2) )
{

}

RooAbsReal* OniaMassFitter::fit()
{
    dcball.fitTo(dataset, RooFit::Hesse(kTRUE),RooFit::Timer(kTRUE),RooFit::Extended());
    return &dcball;
}

RooDataSet* OniaMassFitter::getDataset()
{
    return &dataset;
}

RooRealVar* OniaMassFitter::getVar()
{
    return &mass;
}

//Crystal ball member functions.

CrystalBall::CrystalBall(RooRealVar& var, const char* name):
    mean(   Form("mean_%s",name),"mean of gaussian PDF",S1MEANMASS,S1MEANMASS_MIN,S1MEANMASS_MAX),
    sigma(  Form("sigma_%s",name),"width of gaussian",S1SIGMAMASS, S1SIGMAMASS_MIN, S1SIGMAMASS_MAX),
    alpha(  Form("alpha_%s",name),"tail shift", S1ALPHAMASS,S1ALPHAMASS_MIN,S1ALPHAMASS_MAX),
    n(      Form("n_%s",name),"power order",S1NMASS, S1NMASS_MIN, S1NMASS_MAX),
    cBall(  Form("cball_%s",name),"crystalBall",var,mean,sigma,alpha,n)
{

}

RooCBShape* CrystalBall::getCB()
{
    return &cBall;
}