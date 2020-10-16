

#include "OniaMassFit.h"

using namespace std;

//OniaMassFitter member functions.

OniaMassFitter::OniaMassFitter(TTree* tree_,const kineCutParam* kineCut_):
    kineCut(kineCut_),kineCutStr(kineCutExp(kineCut_)),tree(tree_),
    fs("fs","Crystal ball 1 coeff", 0.5, 0.0, 1.0),
    nSig("nSig","Upsilon Signal", 100001.0, 0.0, 100000000.0),
    nBkg("nBkg","Bkg signal", 10000.0, 0.0, 100000000.0),
    mass("mass","onia mass",kineCut->massLow,kineCut->massHigh,"GeV/c^{2}"),
    pT("pT","momentum",kineCut->ptLow,kineCut->ptHigh,"GeV/c"),
    y("y","rapidity",kineCut->yLow,kineCut->yHigh),
    dataset("dataset","mass dataset",tree_, RooArgSet(mass,pT,y),kineCutStr.data()),
    cball1(mass,"1"),
    cball2(mass,"2"),
    bkg(mass,"bkg"),
    dcball("dcb","double crystal ball", RooArgList(*(cball1.getCB()),*(cball2.getCB()) ),RooArgList(fs) ),
    dcballbkg("dcb_fit","double crystal ball + Bkg", RooArgList(dcball,*(bkg.getChev()) ),RooArgList(nSig,nBkg) )
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
    results=dcballbkg.fitTo(dataset,RooFit::Save(),RooFit::Range(kineCut->massLow,kineCut->massHigh), RooFit::Hesse(),RooFit::Timer(),RooFit::Extended());
    return &dcballbkg;
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

//Chevychev2

Chevychev2::Chevychev2(RooRealVar& var,const char* name):
    ch4_k1("ch4_k1","ch4_k1",0.2,-12,12),
    ch4_k2("ch4_k2","ch4_k1",-0.1,-12,12),
    chev(name,"Background",var,RooArgSet(ch4_k1,ch4_k2))
{

}

RooChebychev* Chevychev2::getChev()
{
    return &chev;
}