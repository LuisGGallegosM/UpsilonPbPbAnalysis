

#include "OniaMassFit.h"

using namespace std;

//OniaMassFitter member functions.

OniaMassFitter::OniaMassFitter(TTree* tree_,const kineCutParam* kineCut_):
    kineCut(kineCut_),kineCutStr(kineCutExp(kineCut_)),tree(tree_),
    coeff1("c1","Crystal ball 1 coeff", 10001.0, 0.0, 10000000.0),
    coeff2("c2","Crystal ball 2 coeff", 10000.0, 0.0, 10000000.0),
    mass("mass","onia mass",kineCut->massLow,kineCut->massHigh,"GeV/c^{2}"),
    pT("pT","momentum",kineCut->ptLow,kineCut->ptHigh,"GeV/c"),
    y("y","rapidity",kineCut->yLow,kineCut->yHigh),
    dataset("dataset","mass dataset",tree_, RooArgSet(mass,pT,y),kineCutStr.data()),
    cball1(mass,"1"),
    cball2(mass,"2"),
    dcball("dcb_fit","double crystal ball", RooArgList(*(cball1.getCB()),*(cball2.getCB()) ),RooArgList(coeff1,coeff2) )
{

}

string OniaMassFitter::kineCutExp(const kineCutParam* kineCut)
{
    string str(Form("pT < %.3f && pT > %.3f",kineCut->ptHigh,kineCut->ptLow));
    str.append(Form("&& y < %.3f && y > %.3f",kineCut->yHigh,kineCut->yLow));
    return str;
}

RooAbsReal* OniaMassFitter::fit()
{
    results=dcball.fitTo(dataset,RooFit::Save(),RooFit::Range(kineCut->massLow,kineCut->massHigh), RooFit::Hesse(),RooFit::Timer(),RooFit::Extended());
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

RooFitResult* OniaMassFitter::getResults()
{
    return results;
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