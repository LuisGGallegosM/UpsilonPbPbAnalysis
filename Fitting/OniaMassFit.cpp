

#include "OniaMassFit.h"

using namespace std;

//OniaMassFitter member functions.

OniaMassFitter::OniaMassFitter(TTree* tree_,const kineCutParam* kineCut_,const fitValues* initialGuess):
    kineCut(*kineCut_),initGuess(*initialGuess),tree(tree_),
    nSig("nSig","Upsilon Signal",initGuess.nSig, 0.0, S1_NSIG_MAX),
    nBkg("nBkg","Bkg signal",initGuess.nBkg, 0.0, S1_NBKG_MAX),
    mass("mass","onia mass",kineCut.massLow,kineCut.massHigh,"GeV/c^{2}"),
    dcball(mass,&(initGuess.dcb)),
    bkg(mass,"bkg",initGuess.chk4_k1,initGuess.chk4_k2)
{

}

string OniaMassFitter::getKineCutExpr() const
{
    string str(Form("(pT < %.3f) && (pT > %.3f)",kineCut.ptHigh,kineCut.ptLow));
    str.append(Form("&& (abs(y) < %.3f) && (abs(y) > %.3f)",kineCut.yHigh,kineCut.yLow));
    str.append(Form("&& (pT_mi  > %.3f) && (abs(eta_mi) < %.3f)",kineCut.singleMuPtLow,kineCut.singleMuEtaHigh));
    str.append(Form("&& (pT_pl  > %.3f) && (abs(eta_pl) < %.3f)",kineCut.singleMuPtLow,kineCut.singleMuEtaHigh));
    return str;
}

RooAbsReal* OniaMassFitter::fit()
{
    RooRealVar pT("pT","momentum quarkonia",kineCut.ptLow,kineCut.ptHigh,"GeV/c");
    RooRealVar y("y","rapidity quarkonia",kineCut.yLow,kineCut.yHigh);
    RooRealVar pT_mi("pT_mi","momentum minus muon",kineCut.ptLow,kineCut.ptHigh,"GeV/c");
    RooRealVar eta_mi("eta_mi","Eta minus muon",kineCut.singleMuEtaHigh);
    RooRealVar pT_pl("pT_pl","momentum plus muon",kineCut.ptLow,kineCut.ptHigh,"GeV/c");
    RooRealVar eta_pl("eta_pl","Eta plus muon",kineCut.singleMuEtaHigh);

    std::string kineCutExpr = getKineCutExpr();
    
    dataset = std::make_unique<RooDataSet>("dataset","mass dataset",tree, RooArgSet(mass,pT,y,pT_mi,pT_pl,eta_mi,eta_pl),kineCutExpr.data());
    std::cout << "Reduced dataset:\n";
    dataset->Print();
    if (kineCut.bkgOn)
    {
        RooAddPdf* dcballbkg = new RooAddPdf("dcb_fit","double crystal ball + Bkg", RooArgList(*(dcball.getDCB()),*(bkg.getChev()) ),RooArgList(nSig,nBkg) );
        output.reset(dcballbkg);
    }
    else
    {
        RooExtendPdf* signal = new RooExtendPdf("dcb_fit","extended signal",*dcball.getDCB(),nSig);
        output.reset(signal);
    }
    RooFitResult* res=output->fitTo(*dataset,RooFit::Save(),RooFit::Range(kineCut.massLow,kineCut.massHigh), RooFit::Hesse(),RooFit::Timer(),RooFit::Extended());
    results.reset(res);
    return output.get();
    
}

RooDataSet* OniaMassFitter::getDataset() const
{
    return dataset.get();
}

RooRealVar* OniaMassFitter::getVar()
{
    return &mass;
}

RooFitResult* OniaMassFitter::getResults() const
{
    return results.get();
}

//Crystal ball member functions.

CrystalBall::CrystalBall(RooRealVar& var, const char* name, const dcbParam* initial):
    mean(   Form("mean_%s",name),"mean of gaussian PDF",initial->mean,S1_MEAN_MIN,S1_MEAN_MAX),
    sigma(  Form("sigma_%s",name),"width of gaussian",initial->sigma1, S1_SIGMA_MIN, S1_SIGMA_MAX),
    alpha(  Form("alpha_%s",name),"tail shift", initial->alpha,S1_ALPHA_MIN,S1_ALPHA_MAX),
    n(      Form("n_%s",name),"power order",initial->n, S1_N_MIN, S1_N_MAX),
    cBall(  Form("cball_%s",name),"crystalBall",var,mean,sigma,alpha,n)
{

}

RooCBShape* CrystalBall::getCB()
{
    return &cBall;
}

//Chevychev2

Chevychev2::Chevychev2(RooRealVar& var,const char* name,float k1,float k2):
    ch4_k1("ch4_k1","ch4_k1",k1,S1_CH4_MIN,S1_CH4_MAX),
    ch4_k2("ch4_k2","ch4_k2",k2,S1_CH4_MIN,S1_CH4_MAX),
    chev(name,"Background",var,RooArgSet(ch4_k1,ch4_k2))
{

}

RooChebychev* Chevychev2::getChev()
{
    return &chev;
}

//Double CrystalBall

DoubleCrystalBall::DoubleCrystalBall(RooRealVar& var, const dcbParam* initial):
    CrystalBall(var,"1",initial),
    mean_2(   "mean_2", "1.0*@0",RooArgList(mean)),
    x(        "x","sigma ratio",initial->x,0.0f,1.0f),
    sigma_2(  "sigma_2","@0*@1",RooArgList(x,sigma)),
    alpha_2(  "alpha_2","1.0*@0",RooArgList(alpha)),
    n_2(      "n_2",    "1.0*@0",RooArgList(n)),
    f(       "fs",     "Crystal ball ratio", initial->f, 0.0f, 1.0f),
    cBall_2(  "cball_2","crystalBall",var,mean_2,sigma_2,alpha_2,n_2),
    dcball(   "dcb",    "double crystal ball", RooArgList(cBall,cBall_2),RooArgList(f) )
{

}

RooAbsPdf* DoubleCrystalBall::getDCB()
{
    return &dcball;
}