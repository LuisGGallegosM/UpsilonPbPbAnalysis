

#include "OniaMassFit.h"

#include "RooAddPdf.h"
#include "RooChebychev.h"
#include "RooFormulaVar.h"
#include "RooExtendPdf.h"

const char mass_name[] = "reco_mass";
const char pT_name[] = "reco_pT";
const char y_name[] ="reco_y";

//OniaMassFitter member functions.

OniaMassFitter::OniaMassFitter(TTree* tree_,const ParameterGroup* fitConf):
    config(*fitConf),tree(tree_),
    nSig_Y1S("nSig_Y1S","Upsilon Signal",
        config.getFloat("nSigY1S.value"), config.getFloat("nSigY1S.low"), config.getFloat("nSigY1S.high")),
    nBkg("nBkg","Bkg signal",
        config.getFloat("nBkg.value"), config.getFloat("nBkg.low"), config.getFloat("nBkg.high")),
    mass(mass_name,"onia mass",
        config.getFloat("mass.low"),config.getFloat("mass.high"),"GeV/c^{2}"),
    dcball1(mass,"Y1S",config.get("dcb")),
    bkg()
{
    BkgFunc* b =BkgFactory(mass,&config);
    bkg.reset(b);
}

OniaMassFitter::~OniaMassFitter() { }

std::string OniaMassFitter::getKineCutExpr() const
{
    const ParameterGroup* cut = config.get("cut");
    std::string str(Form("((%s) < %.3f) && ((%s) > %.3f)",pT_name,cut->getFloat("pt.high"),pT_name,cut->getFloat("pt.low")));
    str.append(Form(" && (abs(%s) < %.3f) && (abs(%s) > %.3f)",y_name,cut->getFloat("y.high"),y_name,cut->getFloat("y.low")));
    if(0)
    {
        str.append(" && (jt_pt < 35) && (jt_pt > 25) ");
        str.append(" && (abs(jt_eta) < 2.0 )");
    }
    return str;
}

void OniaMassFitter::combinePdf()
{
    if (config.getString("bkg.type")!="none")
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
    //RooRealVar z("z", "z",0.0,1.0);
    //RooRealVar jt_pt("jt_pt","jet momentum",0,100,"GeV/c");
    //RooRealVar jt_eta("jt_eta","jet psedorapidity",0.0,4.0);

    RooDataSet* datas= new RooDataSet("dataset","mass dataset",tree, RooArgSet(mass,y,pT),getKineCutExpr().data());
    dataset.reset(datas);
    std::cout << "Reduced dataset:\n";
    dataset->Print();

    combinePdf();

    RooFitResult* res=
        output->fitTo(  *dataset,RooFit::Save(),
                        RooFit::Range(config.getFloat("mass.low"),config.getFloat("mass.high")), 
                        RooFit::Hesse(),
                        RooFit::Timer(),
                        RooFit::Extended());
    results.reset(res);

    return output.get();
    
}

ParameterGroup OniaMassFitter::getFitParams() const
{
    ParameterGroup output;

    output.setBool("moreUpsilon",false);
    ParameterWrite(output,nSig_Y1S,"nSigY1S");
    ParameterWrite(output,nBkg,"nBkg");
    output.addGroup(bkg->getBkgParams(),"bkg");
    output.addGroup(dcball1.getFitParams(),"dcb");

    return output;
}

//OniaMassFitter2

OniaMassFitter2::OniaMassFitter2(TTree* tree_,const ParameterGroup* fitConf):
    OniaMassFitter(tree_,fitConf),
    
    nSig_Y2S("nSig_Y2S","Upsilon Signal Y2S",
        config.getFloat("nSigY2S.value"), config.getFloat("nSigY2S.low"), config.getFloat("nSigY2S.high")),
    nSig_Y3S("nSig_Y3S","Upsilon Signal Y3S",
        config.getFloat("nSigY3S.value"), config.getFloat("nSigY3S.low"), config.getFloat("nSigY3S.high")),
    dcball2(mass,"Y2S",dcball1,RATIO_Y2S),
    dcball3(mass,"Y3S",dcball1,RATIO_Y3S)
{
}

ParameterGroup OniaMassFitter2::getFitParams() const
{
    ParameterGroup output = OniaMassFitter::getFitParams();

    output.setBool("moreUpsilon",true);
    ParameterWrite(output,nSig_Y2S,"nSigY1S");
    ParameterWrite(output,nSig_Y3S,"nSigY1S");

    return output;
}

void OniaMassFitter2::combinePdf()
{
    if (config.getString("bkg.type")!="none")
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

std::unique_ptr<OniaMassFitter> createMassFitter(TTree* input, const ParameterGroup* config)
{
    std::unique_ptr<OniaMassFitter> massFitter;
    if (config->getBool("moreUpsilon"))
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