

#include "OniaMassFit.h"

#include "RooAddPdf.h"
#include "RooChebychev.h"
#include "RooFormulaVar.h"
#include "RooExtendPdf.h"

using namespace std;

//OniaMassFitter member functions.

OniaMassFitter::OniaMassFitter(TTree* tree_,const fitConfig* fitConf):
    config(*fitConf),tree(tree_),
    nSig("nSig","Upsilon Signal",config.initialValues.nSig, 0.0f, S1_NSIG_MAX),
    nBkg("nBkg","Bkg signal",config.initialValues.nBkg, 0.0f, S1_NBKG_MAX),
    mass("mass","onia mass",config.massLow,config.massHigh,"GeV/c^{2}"),
    dcball(mass,&(config.initialValues.dcb)),
    bkg(mass,"bkg",config.initialValues.chk4_k1,config.initialValues.chk4_k2)
{

}

string OniaMassFitter::getKineCutExpr() const
{
    string str(Form("(pT < %.3f) && (pT > %.3f)",config.cut.ptHigh,config.cut.ptLow));
    str.append(Form(" && (abs(y) < %.3f) && (abs(y) > %.3f)",config.cut.yHigh,config.cut.yLow));
    return str;
}

RooAbsReal* OniaMassFitter::fit()
{
    RooRealVar pT("pT","momentum quarkonia",0,100,"GeV/c");
    RooRealVar y("y","rapidity quarkonia",-5,5);
    RooDataSet* datas= new RooDataSet("dataset","mass dataset",tree, RooArgSet(mass,pT,y),getKineCutExpr().data());
    dataset.reset(datas);
    std::cout << "Reduced dataset:\n";
    dataset->Print();
    if (config.bkgOn)
    {
        RooAddPdf* dcballbkg = new RooAddPdf("dcb_fit","double crystal ball + Bkg", RooArgList(*(dcball.getDCB()),*(bkg.getChev()) ),RooArgList(nSig,nBkg) );
        output.reset(dcballbkg);
    }
    else
    {
        RooExtendPdf* signal = new RooExtendPdf("dcb_fit","extended signal",*dcball.getDCB(),nSig);
        output.reset(signal);
    }
    RooFitResult* res=output->fitTo(*dataset,RooFit::Save(),RooFit::Range(config.massLow,config.massHigh), RooFit::Hesse(),RooFit::Timer(),RooFit::Extended());
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