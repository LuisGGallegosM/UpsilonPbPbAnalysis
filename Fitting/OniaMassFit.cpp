

#include "OniaMassFit.h"

#include "RooAddPdf.h"
#include "RooChebychev.h"
#include "RooFormulaVar.h"
#include "RooExtendPdf.h"

using namespace std;

//OniaMassFitter member functions.

OniaMassFitter::OniaMassFitter(TTree* tree_,const fitConfig* fitConf):
    config(*fitConf),tree(tree_),
    nSig_Y1S("nSig_Y1S","Upsilon Signal",config.getInitValues()->getNSigY1S(), 0.0f, S1_NSIG_MAX),
    nBkg("nBkg","Bkg signal",config.getInitValues()->getNBkg(), 0.0f, S1_NBKG_MAX),
    mass("mass","onia mass",config.getMassLow(),config.getMassHigh(),"GeV/c^{2}"),
    dcball1(mass,"Y1S",config.getInitValues()->getDCBParams()),
    bkg()
{
    BkgFunc* b =nullptr;
    BkgParams::BkgType bkgType=config.getInitValues()->getBkgType();

    switch (bkgType)
    {
        case BkgParams::BkgType::chev:
        b = new Chevychev2(mass,"bkg",config.getInitValues()->getChk4_k1(),config.getInitValues()->getChk4_k2());
        break;

        case BkgParams::BkgType::special:
        b = new SpecialBkg(mass,"bkg",config.getInitValues()->getMu(),
                        config.getInitValues()->getSigma(),config.getInitValues()->getLambda());
        break;

        case BkgParams::BkgType::none:
        b = nullptr;
        break;
        
        default:
        throw invalid_argument("\nError: bkgType argument not valid\n");
    }
    if (b!=nullptr)
        bkg.reset(b);
}

OniaMassFitter::~OniaMassFitter() { }

string OniaMassFitter::getKineCutExpr() const
{
    string str(Form("(pT < %.3f) && (pT > %.3f)",config.getCut()->getPtHigh(),config.getCut()->getPtLow()));
    str.append(Form(" && (abs(y) < %.3f) && (abs(y) > %.3f)",config.getCut()->getYHigh(),config.getCut()->getYLow()));
    return str;
}

void OniaMassFitter::combinePdf()
{
    if (config.isBkgOn())
    {
        RooAddPdf* dcballbkg = new RooAddPdf("dcb_fit","double crystal ball + Bkg", RooArgList(*(dcball1.getDCB()),*(bkg->getFunc()) ),RooArgList(nSig_Y1S,nBkg) );
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

    RooFitResult* res=output->fitTo(*dataset,RooFit::Save(),RooFit::Range(config.getMassLow(),config.getMassHigh()), RooFit::Hesse(),RooFit::Timer(),RooFit::Extended());
    results.reset(res);

    return output.get();
    
}

void OniaMassFitter::getFitParams(fitParams* resultParams)
{
    resultParams->setNSig(nSig_Y1S.getVal(),0.0f,0.0f);
    resultParams->setNBkg(nBkg.getVal());
    Chevychev2* chev= dynamic_cast<Chevychev2*>(bkg.get());
    if (chev!=nullptr)
        resultParams->setChk4(chev->getCh4_k1()->getVal(),chev->getCh4_k2()->getVal());
        else
        {
            SpecialBkg* spBkg= dynamic_cast<SpecialBkg*>(bkg.get());
            resultParams->setSpBkg(spBkg->getMu()->getVal(),spBkg->getSigma()->getVal(),spBkg->getLambda()->getVal());
        }

    dcbParam* dcb=resultParams->getDCBParams();
    dcball1.getFitParams(dcb);
}

//OniaMassFitter2


OniaMassFitter2::OniaMassFitter2(TTree* tree_,const fitConfig* fitConf):
    OniaMassFitter(tree_,fitConf),
    nSig_Y2S("nSig_Y2S","Upsilon Signal Y2S",config.getInitValues()->getNSigY2S(), 0.0f, S1_NSIG_MAX),
    nSig_Y3S("nSig_Y3S","Upsilon Signal Y3S",config.getInitValues()->getNSigY3S(), 0.0f, S1_NSIG_MAX),
    dcball2(mass,"Y2S",dcball1,RATIO_Y2S),
    dcball3(mass,"Y3S",dcball1,RATIO_Y3S)
{
}

void OniaMassFitter2::getFitParams(fitParams* resultParams)
{
    OniaMassFitter::getFitParams(resultParams);
    resultParams->setNSig(nSig_Y1S.getVal(),nSig_Y2S.getVal(),nSig_Y3S.getVal());
}

void OniaMassFitter2::combinePdf()
{
    if (config.isBkgOn())
    {
        RooAddPdf* dcballbkg = 
            new RooAddPdf("dcb_fit","3 double crystal ball + Bkg", 
                    RooArgList(*dcball1.getDCB(),*dcball2.getDCB(),*dcball3.getDCB(),*(bkg->getFunc()) ),
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