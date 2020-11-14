

#include "OniaMassFit.h"

#include "RooAddPdf.h"
#include "RooChebychev.h"
#include "RooFormulaVar.h"
#include "RooExtendPdf.h"

using namespace std;

//OniaMassFitter member functions.

OniaMassFitter::OniaMassFitter(TTree* tree_,const fitConfig* fitConf):
    config(*fitConf),tree(tree_),
    nSig_Y1S("nSig_Y1S","Upsilon Signal",config.initialValues.nSigY1S, 0.0f, S1_NSIG_MAX),
    nBkg("nBkg","Bkg signal",config.initialValues.nBkg, 0.0f, S1_NBKG_MAX),
    mass("mass","onia mass",config.massLow,config.massHigh,"GeV/c^{2}"),
    dcball1(mass,"Y1S",&(config.initialValues.dcb)),
    bkg(mass,"bkg",config.initialValues.chk4_k1,config.initialValues.chk4_k2)
{

}

OniaMassFitter::~OniaMassFitter() { }

string OniaMassFitter::getKineCutExpr() const
{
    string str(Form("(pT < %.3f) && (pT > %.3f)",config.cut.ptHigh,config.cut.ptLow));
    str.append(Form(" && (abs(y) < %.3f) && (abs(y) > %.3f)",config.cut.yHigh,config.cut.yLow));
    return str;
}

void OniaMassFitter::combinePdf()
{
    if (config.bkgOn)
    {
        RooAddPdf* dcballbkg = new RooAddPdf("dcb_fit","double crystal ball + Bkg", RooArgList(*(dcball1.getDCB()),*(bkg.getChev()) ),RooArgList(nSig_Y1S,nBkg) );
        output.reset(dcballbkg);
    }
    else
    {
        RooExtendPdf* signal = new RooExtendPdf("dcb_fit","extended signal",*dcball1.getDCB(),nSig_Y1S);
        output.reset(signal);
    }
}

RooAbsReal* OniaMassFitter::fit()
{
    RooRealVar pT("pT","momentum quarkonia",0,100,"GeV/c");
    RooRealVar y("y","rapidity quarkonia",-5,5);
    RooDataSet* datas= new RooDataSet("dataset","mass dataset",tree, RooArgSet(mass,pT,y),getKineCutExpr().data());
    dataset.reset(datas);
    std::cout << "Reduced dataset:\n";
    dataset->Print();

    combinePdf();

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


OniaMassFitter2::OniaMassFitter2(TTree* tree_,const fitConfig* fitConf):
    OniaMassFitter(tree_,fitConf),
    nSig_Y2S("nSig_Y2S","Upsilon Signal Y2S",config.initialValues.nSigY2S, 0.0f, S1_NSIG_MAX),
    nSig_Y3S("nSig_Y3S","Upsilon Signal Y3S",config.initialValues.nSigY3S, 0.0f, S1_NSIG_MAX),
    dcball2(mass,"Y2S",dcball1,RATIO_Y2S),
    dcball3(mass,"Y3S",dcball1,RATIO_Y3S)
{
}

void OniaMassFitter2::combinePdf()
{

    
    if (config.bkgOn)
    {
        RooAddPdf* dcballbkg = 
            new RooAddPdf("dcb_fit","3 double crystal ball + Bkg", 
                    RooArgList(*dcball1.getDCB(),*dcball2.getDCB(),*dcball3.getDCB(),*(bkg.getChev()) ),
                    RooArgList(nSig_Y1S,nSig_Y2S,nSig_Y3S,nBkg) );
        output.reset(dcballbkg);
    }
    else
    {
        RooAddPdf* dcball = 
            new RooAddPdf("dcb_fit","3 double crystal ball + Bkg", 
                    RooArgList(*dcball1.getDCB(),*dcball2.getDCB(),*dcball3.getDCB()),
                    RooArgList(nSig_Y1S,nSig_Y2S,nSig_Y3S) );
        output.reset(dcball);
    }
}