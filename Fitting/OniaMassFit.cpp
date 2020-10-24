

#include "OniaMassFit.h"

using namespace std;

//OniaMassFitter member functions.

OniaMassFitter::OniaMassFitter(TTree* tree_,const kineCutParam* kineCut_):
    kineCut(kineCut_),kineCutStr(kineCutExp(kineCut_)),tree(tree_),
    nSig("nSig","Upsilon Signal", 0.0, 10000000.0),
    nBkg("nBkg","Bkg signal", 0.0, 1000000.0),
    mass("mass","onia mass",kineCut->massLow,kineCut->massHigh,"GeV/c^{2}"),
    pT("pT","momentum quarkonia",kineCut->ptLow,kineCut->ptHigh,"GeV/c"),
    y("y","rapidity quarkonia",kineCut->yLow,kineCut->yHigh),
    pT_mi("pT_mi","momentum minus muon",kineCut->ptLow,kineCut->ptHigh,"GeV/c"),
    pT_pl("pT_pl","momentum plus muon",kineCut->ptLow,kineCut->ptHigh,"GeV/c"),
    eta_mi("eta_mi","Eta minus muon",kineCut->singleMuEtaHigh),
    eta_pl("eta_pl","Eta plus muon",kineCut->singleMuEtaHigh),

    dataset("dataset","mass dataset",tree_, RooArgSet(mass,pT,y,pT_mi,pT_pl,eta_mi,eta_pl),kineCutStr.data()),
    dcball(mass,"1","2"),
    bkg(mass,"bkg")
{

}

string OniaMassFitter::kineCutExp(const kineCutParam* kineCut)
{
    string str(Form("(pT < %.3f) && (pT > %.3f)",kineCut->ptHigh,kineCut->ptLow));
    str.append(Form("&& (abs(y) < %.3f) && (abs(y) > %.3f)",kineCut->yHigh,kineCut->yLow));
    str.append(Form("&& (pT_mi  > %.3f) && (abs(eta_mi) < %.3f)",kineCut->singleMuPtLow,kineCut->singleMuEtaHigh));
    str.append(Form("&& (pT_pl  > %.3f) && (abs(eta_pl) < %.3f)",kineCut->singleMuPtLow,kineCut->singleMuEtaHigh));
    return str;
}

RooAbsReal* OniaMassFitter::fit(bool bkgOn)
{
    if (bkgOn)
    {
        RooAddPdf* dcballbkg = new RooAddPdf("dcb_fit","double crystal ball + Bkg", RooArgList(*(dcball.getDCB()),*(bkg.getChev()) ),RooArgList(nSig,nBkg) );
        output.reset(dcballbkg);
    }
    else
    {
        RooExtendPdf* signal = new RooExtendPdf("dcb_fit","extended signal",*dcball.getDCB(),nSig);
        output.reset(signal);
    }
    results=output->fitTo(dataset,RooFit::Save(),RooFit::Range(kineCut->massLow,kineCut->massHigh), RooFit::Hesse(),RooFit::Timer(),RooFit::Extended());
    return output.get();
    
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
    ch4_k1("ch4_k1","ch4_k1",0.2,-4,4),
    ch4_k2("ch4_k2","ch4_k1",-0.1,-4,4),
    chev(name,"Background",var,RooArgSet(ch4_k1,ch4_k2))
{

}

RooChebychev* Chevychev2::getChev()
{
    return &chev;
}

//Double CrystalBall

DoubleCrystalBall::DoubleCrystalBall(RooRealVar& var, const char* name1, const char* name2):
    CrystalBall(var,name1),
    mean_2(   Form("mean_%s",name2),"1.0*@0",RooArgList(mean)),
    sigma_2(  Form("sigma_%s",name2),"width of gaussian",S1SIGMAMASS*0.5, S1SIGMAMASS_MIN, S1SIGMAMASS_MAX),
    alpha_2(  Form("alpha_%s",name2),"1.0*@0",RooArgList(alpha)),
    n_2(      Form("n_%s",name2),"1.0*@0",RooArgList(n)),
    fs("fs","Crystal ball ratio", 0.5, 0.0, 1.0),
    cBall_2(  Form("cball_%s",name2),"crystalBall",var,mean_2,sigma_2,alpha_2,n_2),
    dcball("dcb","double crystal ball", RooArgList(cBall,cBall_2),RooArgList(fs) )
{

}

RooAbsPdf* DoubleCrystalBall::getDCB()
{
    return &dcball;
}