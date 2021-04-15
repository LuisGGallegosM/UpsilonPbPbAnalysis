

#include"FitFunctions.h"

void ParameterWrite( ParameterGroup& p, const RooRealVar& var, const std::string& name)
{
    p.setFloat(name+".value",var.getVal());
    p.setFloat(name+".low",var.getMin());
    p.setFloat(name+".high", var.getMax());
    p.setFloat(name+".error", var.getError());
    p.setBool(name+".fixed",var.isConstant());
}

//Crystal ball member functions.

CrystalBall::CrystalBall(RooRealVar& var, const char* name, const ParameterGroup* g):
    mean(   Form("mean_%s",name),"mean of gaussian PDF",g->getFloat("mean.value"),g->getFloat("mean.low"),g->getFloat("mean.high")),
    sigma(  Form("sigma_%s",name),"width of gaussian",g->getFloat("sigma.value"),g->getFloat("sigma.low"),g->getFloat("sigma.high")),
    alpha(  Form("alpha_%s",name),"tail shift", g->getFloat("alpha.value"),g->getFloat("alpha.low"),g->getFloat("alpha.high")),
    n(      Form("n_%s",name),"power order",g->getFloat("n.value"),g->getFloat("n.low"),g->getFloat("n.high")),
    cBall(  Form("cball_%s",name),"crystalBall",var,mean,sigma,alpha,n)
{
    mean.setConstant(g->getBool("mean.fixed"));
    sigma.setConstant(g->getBool("sigma.fixed"));
    alpha.setConstant(g->getBool("alpha.fixed"));
    n.setConstant(g->getBool("n.fixed"));
}
//Double CrystalBall

DoubleCrystalBall::DoubleCrystalBall(RooRealVar& var,const char* name, const ParameterGroup* g):
    CrystalBall(var,Form("%s_1",name),g),
    mean_2(   Form("mean_%s_2",name), "1.0*@0",RooArgList(mean)),
    x(        Form("x_%s",name),"sigma ratio",g->getFloat("x.value"),g->getFloat("x.low"),g->getFloat("x.high")),
    sigma_2(  Form("sigma_%s_2",name),"@0*@1",RooArgList(x,sigma)),
    alpha_2(  Form("alpha_%s_2",name),"1.0*@0",RooArgList(alpha)),
    n_2(      Form("n_%s_2",name),    "1.0*@0",RooArgList(n)),
    f(        Form("f_%s",name),     "Crystal ball ratio", g->getFloat("f.value"),g->getFloat("f.low"),g->getFloat("f.high")),
    cBall_2(  Form("cball_%s_2",name),"crystalBall",var,mean_2,sigma_2,alpha_2,n_2),
    dcball(   Form("dcb_%s",name),    "double crystal ball", RooArgList(cBall,cBall_2),RooArgList(f) )
{

}

ParameterGroup DoubleCrystalBall::getFitParams() const
{
    ParameterGroup p;
    ParameterWrite(p,x,"x");
    ParameterWrite(p,f,"f");
    return p;
}

//Slave Crystal ball

CrystalBallSlave::CrystalBallSlave(RooRealVar& var, CrystalBall& cball, const char* name,float ratio):
    mean(   Form("mean_%s",name), Form( "%f *@0",ratio),RooArgList(cball.mean)),
    sigma(  Form("sigma_%s",name),Form( "%f *@0",ratio),RooArgList(cball.sigma)),
    alpha(  Form("alpha_%s",name),      "1.0*@0", RooArgList(cball.alpha)),
    n(      Form("n_%s",name),          "1.0*@0", RooArgList(cball.n)),
    cBall(  Form("cball_%s",name),      "crystalBall",var,mean,sigma,alpha,n)
{

}

//Double CrystalBall Slave

DoubleCrystalBallSlave::DoubleCrystalBallSlave(RooRealVar& var,const char* name,DoubleCrystalBall& doublecb,float ratio):
    CrystalBallSlave(var,doublecb,name,ratio),
    mean_2(   Form("mean_%s_2",name),   "1.0*@0",RooArgList(mean)),
    x(        Form("x_%s",name),        "1.0*@0",RooArgList(doublecb.x)),
    sigma_2(  Form("sigma_%s_2",name),  "@0*@1", RooArgList(doublecb.x,sigma)),
    alpha_2(  Form("alpha_%s_2",name),  "1.0*@0",RooArgList(alpha)),
    n_2(      Form("n_%s_2",name),      "1.0*@0",RooArgList(n)),
    f(        Form("f_%s",name),        "1.0*@0",RooArgList(doublecb.f)),
    cBall_2(  Form("cball_%s_2",name),  "crystalBall",var,mean_2,sigma_2,alpha_2,n_2),
    dcball(   Form("dcb_%s",name),      "double crystal ball", RooArgList(cBall,cBall_2),RooArgList(f) )
{

}

//BkgFunc

ParameterGroup BkgFunc::getBkgParams() const
{
    ParameterGroup p;
    p.setString("type","none");
    return p;
}

//Chevychev2

Chevychev2::Chevychev2(RooRealVar& var,const char* name,float* k, float* low, float* high):
    ch4_k1("chk4_k1","chk4_k1",k[0],low[0],high[0]),
    ch4_k2("chk4_k2","chk4_k2",k[1],low[1],high[1]),
    chev(name,"Background",var,RooArgSet(ch4_k1,ch4_k2))
{

}

ParameterGroup Chevychev2::getBkgParams() const
{
    ParameterGroup p;
    ParameterWrite(p,ch4_k1,"chk4_k1");
    ParameterWrite(p,ch4_k2,"chk4_k2");
    p.setString("type","chev");
    return p;
}

//SpecialBkg

SpecialBkg::SpecialBkg(RooRealVar& var,const char* name,float* initial,float* low, float* high):
    mu(Form("mu_%s",name),"err_mu",initial[2],  low[2], high[2]),
    sigma(Form("sigma_%s",name),"err_sigma", initial[1], low[1],high[1]),
    lambda(Form("lambda_%s",name),"m_lambda",  initial[0], low[0],high[0]),
    bkgPdf()
{
    RooGenericPdf* pdf=
        new RooGenericPdf(name,"Background","TMath::Exp(-@0/@1)*(TMath::Erf((@0-@2)/(TMath::Sqrt(2)*@3))+1)*0.5",
                           RooArgList(var, lambda, mu, sigma));
    bkgPdf.reset(pdf);
}

ParameterGroup SpecialBkg::getBkgParams() const
{
    ParameterGroup p;
    ParameterWrite(p,mu,"mu");
    ParameterWrite(p,sigma,"sigma");
    ParameterWrite(p,lambda,"lambda");
    p.setString("type","special");
    return p;
}

//ExponentialBkg

ExponentialBkg::ExponentialBkg(RooRealVar& var,const char* name,float* initial,float* low, float* high):
lambda(Form("lambda_%s",name),"m_lambda",  initial[0], low[0],high[0]),
bkgPdf()
{
    RooGenericPdf* pdf=
        new RooGenericPdf(name,"Background","TMath::Exp(-@0/@1)",
                           RooArgList(var, lambda));
    bkgPdf.reset(pdf);
}

ParameterGroup ExponentialBkg::getBkgParams() const
{
    ParameterGroup p;
    ParameterWrite(p,lambda,"lambda");
    p.setString("type","exp");
    return p;
}

//ExpChevBkg

ExpChev2Bkg::ExpChev2Bkg(RooRealVar& var,const char* name,float* initial, float* low, float* high):
    ch4_k1("chk4_k1","chk4_k1",initial[0],low[0],high[0]),
    ch4_k2("chk4_k2","chk4_k2",initial[1],low[1],high[1]),
    bkgPdf()
{
    RooGenericPdf* pdf=
        new RooGenericPdf(name,"Background","TMath::Exp(@2*(2.0*@0*@0-1.0)+@1*@0+1.0)",
                           RooArgList(var, ch4_k1,ch4_k2));
    bkgPdf.reset(pdf);
}
ParameterGroup ExpChev2Bkg::getBkgParams() const
{
    ParameterGroup p;
    ParameterWrite(p,ch4_k1,"chk4_k1");
    ParameterWrite(p,ch4_k2,"chk4_k2");
    p.setString("type","chev");
    return p;
}

enum class BkgType {error, none, chev, special,exponential,expChev2 };

const std::map<std::string,BkgType> bkgNames =
{
    {"none",BkgType::none},
     {"chev",BkgType::chev},
    {"special",BkgType::special},
    {"exponential",BkgType::exponential},
    {"expChev2",BkgType::expChev2}
};

void setArray(float* initials, float* low, float* high,const std::string& name, int index,const ParameterGroup& config)
{
    initials[index]=config.getFloat(name+".value");
    low[index]=config.getFloat(name+".low");
    high[index]=config.getFloat(name+".high");
}

BkgFunc* BkgFactory(RooRealVar& var, const ParameterGroup& config)
{
    
    BkgFunc* b= nullptr;
    BkgType bkgType = bkgNames.at(config.getString("type"));
    float initials[3];
    float low[3];
    float high[3];
    switch (bkgType)
    {
        case BkgType::chev:
        setArray(initials,low,high,"chk4_k1",0,config);
        setArray(initials,low,high,"chk4_k2",1,config);
        b = new Chevychev2(var,"bkg",initials,low,high);
        break;

        case BkgType::expChev2:
        setArray(initials,low,high,"chk4_k1",0,config);
        setArray(initials,low,high,"chk4_k2",1,config);
        b = new ExpChev2Bkg(var,"bkg",initials,low,high);
        break;

        case BkgType::special:
        setArray(initials,low,high,"lambda",0,config);
        setArray(initials,low,high,"sigma",1,config);
        setArray(initials,low,high,"mu",2,config);
        b = new SpecialBkg(var,"bkg",initials,low,high);
        break;

        case BkgType::exponential:
        setArray(initials,low,high,"lambda",0,config);
        b = new ExponentialBkg(var,"bkg",initials,low,high);

        break;

        case BkgType::none:
        b = new BkgFunc();
        break;

        default:
        throw std::invalid_argument("\nError: bkgType argument not valid\n");
    }
    return b;
}