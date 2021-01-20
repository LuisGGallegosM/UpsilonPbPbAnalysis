

#include"FitFunctions.h"

//Crystal ball member functions.

CrystalBall::CrystalBall(RooRealVar& var, const char* name, const dcbParam* initial, 
        const dcbParam* low, const dcbParam* high,bool fixAlpha, bool fixN):
    mean(   Form("mean_%s",name),"mean of gaussian PDF",initial->getMean(),low->getMean(),high->getMean()),
    sigma(  Form("sigma_%s",name),"width of gaussian",initial->getSigma(), low->getSigma(), high->getSigma()),
    alpha(  Form("alpha_%s",name),"tail shift", initial->getAlpha(),low->getAlpha(),high->getAlpha()),
    n(      Form("n_%s",name),"power order",initial->getN(), low->getN(), high->getN()),
    cBall(  Form("cball_%s",name),"crystalBall",var,mean,sigma,alpha,n)
{
    if (fixAlpha)
        alpha.setConstant(kTRUE);
    if (fixN)
        n.setConstant(kTRUE);
}

RooCBShape* CrystalBall::getCB()
{
    return &cBall;
}

//Double CrystalBall

DoubleCrystalBall::DoubleCrystalBall(RooRealVar& var,const char* name, const dcbParam* initial, 
        const dcbParam* low, const dcbParam* high,bool fixAlpha, bool fixN):
    CrystalBall(var,Form("%s_1",name),initial,low,high,fixAlpha,fixN),
    mean_2(   Form("mean_%s_2",name), "1.0*@0",RooArgList(mean)),
    x(        Form("x_%s",name),"sigma ratio",initial->getX(),low->getX(),high->getX()),
    sigma_2(  Form("sigma_%s_2",name),"@0*@1",RooArgList(x,sigma)),
    alpha_2(  Form("alpha_%s_2",name),"1.0*@0",RooArgList(alpha)),
    n_2(      Form("n_%s_2",name),    "1.0*@0",RooArgList(n)),
    f(        Form("f_%s",name),     "Crystal ball ratio", initial->getF(), low->getF(), high->getF()),
    cBall_2(  Form("cball_%s_2",name),"crystalBall",var,mean_2,sigma_2,alpha_2,n_2),
    dcball(   Form("dcb_%s",name),    "double crystal ball", RooArgList(cBall,cBall_2),RooArgList(f) )
{

}

dcbParam DoubleCrystalBall::getFitParams() const
{
    dcbParam p;
    p.setParams(mean.getVal(),alpha.getVal(),n.getVal(),sigma.getVal(),x.getVal(),f.getVal());
    return p;
}

dcbParam DoubleCrystalBall::getFitParamsErrors() const
{
    dcbParam p;
    p.setParams(mean.getError(),alpha.getError(),n.getError(),sigma.getError(),x.getError(),f.getError());
    return p;
}

dcbParam DoubleCrystalBall::getFitParamsHigh() const
{
    dcbParam p;
    p.setParams(mean.getMax(),alpha.getMax(),n.getMax(),sigma.getMax(),x.getMax(),f.getMax());
    return p;
}

dcbParam DoubleCrystalBall::getFitParamsLow() const
{
    dcbParam p;
    p.setParams(mean.getMin(),alpha.getMin(),n.getMin(),sigma.getMin(),x.getMin(),f.getMin());
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

RooCBShape* CrystalBallSlave::getCB()
{
    return &cBall;
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

RooAbsPdf* DoubleCrystalBallSlave::getDCB()
{
    return &dcball;
}

//BkgFunc

BkgParams BkgFunc::getBkgParams()
{
    BkgParams p;
    p.setBkgType(BkgParams::BkgType::none);
    return p;
}

BkgParams BkgFunc::getBkgParamsErrors()
{
    BkgParams p;
    p.setBkgType(BkgParams::BkgType::none);
    return p;
}

BkgParams BkgFunc::getBkgParamsHigh()
{
    BkgParams p;
    p.setBkgType(BkgParams::BkgType::none);
    return p;
}

BkgParams BkgFunc::getBkgParamsLow()
{
    BkgParams p;
    p.setBkgType(BkgParams::BkgType::none);
    return p;
}

//Chevychev2

Chevychev2::Chevychev2(RooRealVar& var,const char* name,float* k, float* low, float* high):
    ch4_k1("chk4_k1","chk4_k1",k[0],low[0],high[0]),
    ch4_k2("chk4_k2","chk4_k2",k[1],low[1],high[1]),
    chev(name,"Background",var,RooArgSet(ch4_k1,ch4_k2))
{

}

BkgParams Chevychev2::getBkgParams()
{
    BkgParams p;
    p.setChk4(ch4_k1.getVal(),ch4_k2.getVal());
    p.setBkgType(BkgParams::BkgType::chev);
    return p;
}

BkgParams Chevychev2::getBkgParamsErrors()
{
    BkgParams p;
    p.setChk4(ch4_k1.getError(),ch4_k2.getError());
    p.setBkgType(BkgParams::BkgType::chev);
    return p;
}

BkgParams Chevychev2::getBkgParamsHigh()
{
    BkgParams p;
    p.setChk4(ch4_k1.getMax(),ch4_k2.getMax());
    p.setBkgType(BkgParams::BkgType::chev);
    return p;
}

BkgParams Chevychev2::getBkgParamsLow()
{
    BkgParams p;
    p.setChk4(ch4_k1.getMin(),ch4_k2.getMin());
    p.setBkgType(BkgParams::BkgType::chev);
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

BkgParams SpecialBkg::getBkgParams()
{
    BkgParams p;
    p.setSpBkg(mu.getVal(),sigma.getVal(),lambda.getVal());
    p.setBkgType(BkgParams::BkgType::special);
    return p;
}

BkgParams SpecialBkg::getBkgParamsErrors()
{
    BkgParams p;
    p.setSpBkg(mu.getError(),sigma.getError(),lambda.getError());
    p.setBkgType(BkgParams::BkgType::special);
    return p;
}

BkgParams SpecialBkg::getBkgParamsHigh()
{
    BkgParams p;
    p.setSpBkg(mu.getMax(),sigma.getMax(),lambda.getMax());
    p.setBkgType(BkgParams::BkgType::special);
    return p;
}

BkgParams SpecialBkg::getBkgParamsLow()
{
    BkgParams p;
    p.setSpBkg(mu.getMin(),sigma.getMin(),lambda.getMin());
    p.setBkgType(BkgParams::BkgType::special);
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

BkgParams ExponentialBkg::getBkgParams()
{
    BkgParams p;
    p.setLambda(lambda.getVal());
    p.setBkgType(BkgParams::BkgType::exponential);
    return p;
}

BkgParams ExponentialBkg::getBkgParamsErrors()
{
    BkgParams p;
    p.setLambda(lambda.getError());
    p.setBkgType(BkgParams::BkgType::exponential);
    return p;
}

BkgParams ExponentialBkg::getBkgParamsHigh()
{
    BkgParams p;
    p.setLambda(lambda.getMax());
    p.setBkgType(BkgParams::BkgType::exponential);
    return p;
}

BkgParams ExponentialBkg::getBkgParamsLow()
{
    BkgParams p;
    p.setLambda(lambda.getMin());
    p.setBkgType(BkgParams::BkgType::exponential);
    return p;
}


BkgFunc* BkgFactory(RooRealVar& var, const fitConfig& config)
{
    BkgFunc* b= nullptr;
    BkgParams::BkgType bkgType = config.getBkgType();
    float initials[3];
    float low[3];
    float high[3];
    switch (bkgType)
    {
        case BkgParams::BkgType::chev:
        initials[0]=config.getInitValues()->getChk4_k1();
        initials[1]=config.getInitValues()->getChk4_k2();
        low[0]=config.getInitValues()->getLowLimit().getChk4_k1();
        low[1]=config.getInitValues()->getLowLimit().getChk4_k2();
        high[0]=config.getInitValues()->getHighLimit().getChk4_k1();
        high[1]=config.getInitValues()->getHighLimit().getChk4_k2();
        b = new Chevychev2(var,"bkg",initials,low,high);
        break;

        case BkgParams::BkgType::special:
        initials[0]=config.getInitValues()->getLambda();
        initials[1]=config.getInitValues()->getSigmaBkg();
        initials[2]=config.getInitValues()->getMu();
        low[0]=config.getInitValues()->getLowLimit().getLambda();
        low[1]=config.getInitValues()->getLowLimit().getSigmaBkg();
        low[2]=config.getInitValues()->getLowLimit().getMu();
        high[0]=config.getInitValues()->getHighLimit().getLambda();
        high[1]=config.getInitValues()->getHighLimit().getSigmaBkg();
        high[2]=config.getInitValues()->getHighLimit().getMu();

        b = new SpecialBkg(var,"bkg",initials,low,high);
        break;

        case BkgParams::BkgType::exponential:
        initials[0]=config.getInitValues()->getLambda();
        low[0]=config.getInitValues()->getLowLimit().getLambda();
        high[0]=config.getInitValues()->getHighLimit().getLambda();
        b = new ExponentialBkg(var,"bkg",initials,low,high);

        break;

        case BkgParams::BkgType::none:
        b = new BkgFunc();
        break;

        default:
        throw std::invalid_argument("\nError: bkgType argument not valid\n");
    }
    return b;
}