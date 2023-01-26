

#include "OniaMassFit.h"

#include "RooAddPdf.h"
#include "RooChebychev.h"
#include "RooFormulaVar.h"
#include "RooExtendPdf.h"

#include"../Common/Common.h"

std::string toInternalName(const std::string& name)
{
    if (name=="pt") return "reco_pT";
    if (name=="y") return "abs(reco_y)";
    if (name=="mass") return "reco_mass";
    if (name=="jt_eta") return "abs(jt_eta)";
    return name;
}

std::vector<RooRealVar> getVars()
{
    return std::vector<RooRealVar>
    {
        RooRealVar ("reco_y","rapidity quarkonia",-5,5),
        RooRealVar ("reco_pT","momentum quarkonia",0,100,"GeV/c"),
        RooRealVar ("z", "z",0.0,1.0),
        RooRealVar ("jt_pt","jet momentum",0,100,"GeV/c"),
        RooRealVar ("jt_eta","jet psedorapidity",-4.0,4.0),
        RooRealVar ("accxeff", "weight var",0.0,100.0)
    };
}

std::string getKineCutExpr(const ParameterGroup* cut)
{
    const auto subg = cut->getSubgroupNames();
    std::string str;
    for(const auto& name : subg)
    {
        if ((name=="mass") || (name=="vars")) continue;
        if ( cut->exists( name+ ".low"))
            str+= "(("+ toInternalName(name) +") > "+std::to_string(cut->getFloat(name+ ".low"))+") && ";
        if ( cut->exists( name+ ".high"))
            str+= "(("+ toInternalName(name) +") < "+std::to_string(cut->getFloat(name+ ".high")) + ") && ";
    }
    str.erase(str.end()-4,str.end());
    return str;
}

//OniaMassFitter member functions.

OniaMassFitter::OniaMassFitter(TTree* tree_,const ParameterGroup* fitConf):
    config(*fitConf),tree(tree_),
    nSig_Y1S("nSig_Y1S","Upsilon Signal",
        config.getFloat("signal.nSigY1S.value"), config.getFloat("signal.nSigY1S.low"), config.getFloat("signal.nSigY1S.high")),
    nBkg("nBkg","Bkg signal",
        config.getFloat("bkg.nBkg.value"), config.getFloat("bkg.nBkg.low"), config.getFloat("bkg.nBkg.high")),
    mass( toInternalName("mass").data(),"onia mass",
        config.getFloat("cut.mass.low"),config.getFloat("cut.mass.high"),"GeV/c^{2}"),
    dcball1(nullptr),
    bkg()
{
    std::string type= config.get("signal")->getString("type");
    if (type=="dcb")
    {
        std::cout << "using double crystal ball signal fit.\n";
        dcball1.reset( new DoubleCrystalBall(mass,"Y1S",config.get("signal")) );
    } else if (type == "tcb")
    {
        std::cout << "using triple crystal ball signal fit.\n";
        dcball1.reset( new TripleCrystalBall(mass,"Y1S",config.get("signal")) );
    } else if (type=="cbgauss")
    {
        std::cout << "using crystal ball + gauss signal fit.\n";
        dcball1.reset(new CrystalBallGauss(mass,"Y1S",config.get("signal")));
    } else if (type=="dcbgauss")
    {
        std::cout << "using double crystal ball + gauss signal fit.\n";
        dcball1.reset(new DoubleCrystalBallGauss(mass,"Y1S",config.get("signal")));
    } else throw std::invalid_argument("ERROR: no valid signal type");
    FitFunc* b =BkgFactory(mass,config.get(bkgName));
    bkg.reset(b);
}

OniaMassFitter::~OniaMassFitter() { }

void OniaMassFitter::combinePdf()
{
    if (config.getString("bkg.type")!="none")
    {
        RooAddPdf* dcballbkg = 
            new RooAddPdf(  "dcb_fit",
                            "double crystal ball + Bkg", 
                            RooArgList(*(dcball1->getDCB()),
                            *(bkg->getFunc()) ),
                            RooArgList(nSig_Y1S,nBkg) );
        output.reset(dcballbkg);
    }
    else
    {
        RooExtendPdf* signal = new RooExtendPdf("dcb_fit","extended signal",*dcball1->getDCB(),nSig_Y1S);
        output.reset(signal);
    }
}

RooAbsReal* OniaMassFitter::fit()
{
    auto vars= getVars();
    RooArgSet set;
    set.add(mass);
    for(const auto& var : vars)
    {
        if( tree->GetBranch(var.GetName()) != nullptr )
            set.add(var);
    }
    const char* weightVarName=nullptr;
    if (tree->GetBranch("accxeff")!=nullptr)
    {
        weightVarName="accxeff";
        std::cout << "weight branch detected as " << weightVarName << ".\n";
    }
    
    RooDataSet* datas= new RooDataSet("dataset","mass dataset",tree, set,getKineCutExpr(config.get("cut")).data(),weightVarName);
    dataset.reset(datas);
    std::cout << "Reduced dataset:\n";
    dataset->Print();

    combinePdf();

    RooFitResult* res=
        output->fitTo(  *dataset,RooFit::Save(),
                        RooFit::Range(config.getFloat("cut.mass.low"),config.getFloat("cut.mass.high")), 
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
    ParameterWrite(output,nSig_Y1S,"signal.nSigY1S");
    ParameterWrite(output,nBkg,"bkg.nBkg");
    output.addGroup(bkg->getBkgParams(),bkgName);
    output.addGroup(dcball1->getParams(),"signal");

    return output;
}

//OniaMassFitter2

OniaMassFitter2::OniaMassFitter2(TTree* tree_,const ParameterGroup* fitConf):
    OniaMassFitter(tree_,fitConf),
    
    nSig_Y2S("nSig_Y2S","Upsilon Signal Y2S",
        config.getFloat("signal.nSigY2S.value"), config.getFloat("signal.nSigY2S.low"), config.getFloat("signal.nSigY2S.high")),
    nSig_Y3S("nSig_Y3S","Upsilon Signal Y3S",
        config.getFloat("signal.nSigY3S.value"), config.getFloat("signal.nSigY3S.low"), config.getFloat("signal.nSigY3S.high")),
    dcball2(nullptr),
    dcball3(nullptr)
{
    std::string type= config.get("signal")->getString("type");
    if (type=="dcb")
    {
        dcball2.reset( new DoubleCrystalBallSlave(mass,"Y2S", *dynamic_cast<DoubleCrystalBall*> (dcball1.get()),RATIO_Y2S) );
        dcball3.reset( new DoubleCrystalBallSlave(mass,"Y3S", *dynamic_cast<DoubleCrystalBall*> (dcball1.get()),RATIO_Y3S) );
    } else if (type=="tcb")
    {
        dcball2.reset( new TripleCrystalBallSlave(mass,"Y2S", *dynamic_cast<TripleCrystalBall*> (dcball1.get()),RATIO_Y2S) );
        dcball3.reset( new TripleCrystalBallSlave(mass,"Y3S", *dynamic_cast<TripleCrystalBall*> (dcball1.get()),RATIO_Y3S) );
    } else if (type=="cbgauss")
    {
        dcball2.reset( new CrystalGaussSlave(mass,"Y2S", *dynamic_cast<CrystalBallGauss*> (dcball1.get()),RATIO_Y2S) );
        dcball3.reset( new CrystalGaussSlave(mass,"Y3S", *dynamic_cast<CrystalBallGauss*> (dcball1.get()),RATIO_Y3S) );
    } else if(type == "dcbgauss")
    {
        dcball2.reset( new DoubleCrystalGaussSlave(mass,"Y2S", *dynamic_cast<DoubleCrystalBallGauss*> (dcball1.get()),RATIO_Y2S) );
        dcball3.reset( new DoubleCrystalGaussSlave(mass,"Y3S", *dynamic_cast<DoubleCrystalBallGauss*> (dcball1.get()),RATIO_Y3S) );
    }
    else throw std::invalid_argument("ERROR: no valid signal type");
}

ParameterGroup OniaMassFitter2::getFitParams() const
{
    ParameterGroup output = OniaMassFitter::getFitParams();

    output.setBool("moreUpsilon",true);
    ParameterWrite(output,nSig_Y2S,"signal.nSigY2S");
    ParameterWrite(output,nSig_Y3S,"signal.nSigY3S");

    return output;
}

void OniaMassFitter2::combinePdf()
{
    if (config.getString("bkg.type")!="none")
    {
        RooAddPdf* dcballbkg = 
            new RooAddPdf("dcb_fit","3 double crystal ball + Bkg", 
                    RooArgList(*(dcball1->getDCB()),*dcball2->getDCB(),*dcball3->getDCB(),*(bkg->getFunc()) ),
                    RooArgList(nSig_Y1S,nSig_Y2S,nSig_Y3S,nBkg) );
        output.reset(dcballbkg);
    }
    else
    {
        RooAddPdf* dcball = 
            new RooAddPdf("dcb_fit","3 double crystal ball + Bkg", 
                    RooArgList(*(dcball1->getDCB()),*dcball2->getDCB(),*dcball3->getDCB()),
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