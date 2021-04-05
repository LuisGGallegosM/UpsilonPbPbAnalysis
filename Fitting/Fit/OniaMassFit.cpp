

#include "OniaMassFit.h"

#include "RooAddPdf.h"
#include "RooChebychev.h"
#include "RooFormulaVar.h"
#include "RooExtendPdf.h"

const char mass_name[] = "reco_mass";
const char pT_name[] = "z";
const char y_name[] ="reco_y";

//OniaMassFitter member functions.

OniaMassFitter::OniaMassFitter(TTree* tree_,const fitConfig* fitConf):
    config(*fitConf),tree(tree_),
    nSig_Y1S("nSig_Y1S","Upsilon Signal",config.getInitValues()->getNSigY1S(), config.getInitValues()->getLowLimit().getNSigY1S(), config.getInitValues()->getHighLimit().getNSigY1S()),
    nBkg("nBkg","Bkg signal",config.getInitValues()->getNBkg(),config.getInitValues()->getLowLimit().getNBkg(), config.getInitValues()->getHighLimit().getNBkg()),
    mass(mass_name,"onia mass",config.getMassLow(),config.getMassHigh(),"GeV/c^{2}"),
    dcball1(mass,"Y1S",config.getInitValues()->getDCBParams(),config.getInitValues()->getLowLimit().getDCBParams(),config.getInitValues()->getHighLimit().getDCBParams(),fitConf->getFixAlpha(),fitConf->getFixN()),
    bkg()
{
    BkgFunc* b =BkgFactory(mass,config);
    bkg.reset(b);
}

OniaMassFitter::~OniaMassFitter() { }

std::string OniaMassFitter::getKineCutExpr() const
{
    std::string str(Form("((%s)^2 < %.3f) && ((%s)^2 > %.3f)",pT_name,config.getCut()->getPtHigh(),pT_name,config.getCut()->getPtLow()));
    str.append(Form(" && (abs(%s) < %.3f) && (abs(%s) > %.3f)",y_name,config.getCut()->getYHigh(),y_name,config.getCut()->getYLow()));
    if(1)
    {
        str.append(" && (jt_pt < 35) && (jt_pt > 25) ");
        str.append(" && (abs(jt_eta) < 2.0 )");
    }
    return str;
}

void OniaMassFitter::combinePdf()
{
    if (config.isBkgOn())
    {
        RooAddPdf* dcballbkg = 
            new RooAddPdf(  "dcb_fit",
                            "double crystal ball + Bkg", 
                            RooArgList(*(dcball1.getDCB()),
                            *(bkg->getFunc()) ),
                            RooArgList(nSig_Y1S,nBkg) );
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
    RooRealVar pT(pT_name,"momentum quarkonia",0,100,"GeV/c");
    RooRealVar y(y_name,"rapidity quarkonia",-5,5);
    RooRealVar z("z", "z",0.0,1.0);
    RooRealVar jt_pt("jt_pt","jet momentum",0,100,"GeV/c");
    RooRealVar jt_eta("jt_eta","jet psedorapidity",0.0,4.0);

    RooDataSet* datas= new RooDataSet("dataset","mass dataset",tree, RooArgSet(mass,y,z,jt_pt,jt_eta),getKineCutExpr().data());
    dataset.reset(datas);
    std::cout << "Reduced dataset:\n";
    dataset->Print();

    combinePdf();

    RooFitResult* res=
        output->fitTo(  *dataset,RooFit::Save(),
                        RooFit::Range(config.getMassLow(),
                        config.getMassHigh()), 
                        RooFit::Hesse(),
                        RooFit::Timer(),
                        RooFit::Extended());
    results.reset(res);

    return output.get();
    
}

fitParamsWithErrors OniaMassFitter::getFitParams()
{
    fitParamsWithErrors output;
    dcbParam dcb;
    BkgParams bkgParams;

    //values
    output.setMoreUpsilon(false);
    output.setNSig(nSig_Y1S.getVal());
    output.setNBkg(nBkg.getVal());
    bkgParams=bkg->getBkgParams();
    output.setBkgParams(bkgParams);
    dcb = dcball1.getFitParams();
    output.setDCBParams(dcb);

    //high
    output.getHighLimit().setMoreUpsilon(false);
    output.getHighLimit().setNSig(nSig_Y1S.getMax());
    output.getHighLimit().setNBkg(nBkg.getMax());
    bkgParams=bkg->getBkgParamsHigh();
    output.getHighLimit().setBkgParams(bkgParams);
    dcb= dcball1.getFitParamsHigh();
    output.getHighLimit().setDCBParams(dcb);

    //low
    output.getLowLimit().setMoreUpsilon(false);
    output.getLowLimit().setNSig(nSig_Y1S.getMin());
    output.getLowLimit().setNBkg(nBkg.getMin());
    bkgParams=bkg->getBkgParamsLow();
    output.getLowLimit().setBkgParams(bkgParams);
    dcb= dcball1.getFitParamsLow();
    output.getLowLimit().setDCBParams(dcb);

    //errors
    output.getErrors().setMoreUpsilon(false);
    output.getErrors().setNSig(nSig_Y1S.getError());
    output.getErrors().setNBkg(nBkg.getError());
    bkgParams= bkg->getBkgParamsErrors();
    output.getErrors().setBkgParams(bkgParams);
    dcb = dcball1.getFitParamsErrors();
    output.getErrors().setDCBParams(dcb);

    return output;
}

//OniaMassFitter2

OniaMassFitter2::OniaMassFitter2(TTree* tree_,const fitConfig* fitConf):
    OniaMassFitter(tree_,fitConf),
    
    nSig_Y2S("nSig_Y2S","Upsilon Signal Y2S",config.getInitValues()->getNSigY2S(), 
    config.getInitValues()->getLowLimit().getNSigY2S(), config.getInitValues()->getHighLimit().getNSigY2S()),

    nSig_Y3S("nSig_Y3S","Upsilon Signal Y3S",config.getInitValues()->getNSigY3S(), 
    config.getInitValues()->getLowLimit().getNSigY3S(), config.getInitValues()->getHighLimit().getNSigY3S()),

    dcball2(mass,"Y2S",dcball1,RATIO_Y2S),
    dcball3(mass,"Y3S",dcball1,RATIO_Y3S)
{
}

fitParamsWithErrors OniaMassFitter2::getFitParams()
{
    fitParamsWithErrors output = OniaMassFitter::getFitParams();
    output.setMoreUpsilon(true);
    output.setNSig(nSig_Y1S.getVal(),nSig_Y2S.getVal(),nSig_Y3S.getVal());
    output.getErrors().setNSig(nSig_Y1S.getError(),nSig_Y2S.getError(),nSig_Y3S.getError());
    output.getHighLimit().setNSig(nSig_Y1S.getMax(),nSig_Y2S.getMax(),nSig_Y3S.getMax());
    output.getLowLimit().setNSig(nSig_Y1S.getMin(),nSig_Y2S.getMin(),nSig_Y3S.getMin());
    return output;
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

std::unique_ptr<OniaMassFitter> createMassFitter(TTree* input, const fitConfig* config)
{
    std::unique_ptr<OniaMassFitter> massFitter;
    if (config->isMoreUpsilon())
    {
        //fit with 1S,2S and 3S Upsilon 
        massFitter.reset(new OniaMassFitter2(input, config));
    }
    else
    {
        //fit only 1S Upsilon
        massFitter.reset(new OniaMassFitter(input, config));
    }
    return massFitter;
}