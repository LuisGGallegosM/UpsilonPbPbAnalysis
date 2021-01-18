

#include"FitFunctions.h"

//Crystal ball member functions.

CrystalBall::CrystalBall(RooRealVar& var, const char* name, const dcbParam* initial,bool fixAlpha, bool fixN):
    mean(   Form("mean_%s",name),"mean of gaussian PDF",initial->getMean(),S1_MEAN_MIN,S1_MEAN_MAX),
    sigma(  Form("sigma_%s",name),"width of gaussian",initial->getSigma(), S1_SIGMA_MIN, S1_SIGMA_MAX),
    alpha(  Form("alpha_%s",name),"tail shift", initial->getAlpha(),S1_ALPHA_MIN,S1_ALPHA_MAX),
    n(      Form("n_%s",name),"power order",initial->getN(), S1_N_MIN, S1_N_MAX),
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

DoubleCrystalBall::DoubleCrystalBall(RooRealVar& var,const char* name, const dcbParam* initial,bool fixAlpha, bool fixN):
    CrystalBall(var,Form("%s_1",name),initial,fixAlpha,fixN),
    mean_2(   Form("mean_%s_2",name), "1.0*@0",RooArgList(mean)),
    x(        Form("x_%s",name),"sigma ratio",initial->getX(),0.0f,1.0f),
    sigma_2(  Form("sigma_%s_2",name),"@0*@1",RooArgList(x,sigma)),
    alpha_2(  Form("alpha_%s_2",name),"1.0*@0",RooArgList(alpha)),
    n_2(      Form("n_%s_2",name),    "1.0*@0",RooArgList(n)),
    f(        Form("f_%s",name),     "Crystal ball ratio", initial->getF(), 0.0f, 1.0f),
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

void DoubleCrystalBall::getFitParamsErrors(dcbParam* params)
{
    params->setParams(mean.getError(),alpha.getError(),n.getError(),sigma.getError(),x.getError(),f.getError());
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

//Chevychev2

Chevychev2::Chevychev2(RooRealVar& var,const char* name,float k1,float k2):
    ch4_k1("chk4_k1","chk4_k1",k1,S1_CH4_MIN,S1_CH4_MAX),
    ch4_k2("chk4_k2","chk4_k2",k2,S1_CH4_MIN,S1_CH4_MAX),
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

//SpecialBkg

SpecialBkg::SpecialBkg(RooRealVar& var,const char* name,float mu_,float sigma_, float lambda_):
    mu(Form("mu_%s",name),"err_mu",mu_,  0.0f, BKG_MU_MAX),
    sigma(Form("sigma_%s",name),"err_sigma", sigma_, 0.0f,BKG_SIGMA_MAX),
    lambda(Form("lambda_%s",name),"m_lambda",  lambda_, 0.0f,BKG_LAMBDA_MAX),
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

//ExponentialBkg

ExponentialBkg::ExponentialBkg(RooRealVar& var,const char* name,float lambda_):
lambda(Form("lambda_%s",name),"m_lambda",  lambda_, 0.0f,BKG_LAMBDA_MAX),
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


BkgFunc* BkgFactory(RooRealVar& var, const fitConfig& config)
{
    BkgFunc* b= nullptr;
    BkgParams::BkgType bkgType = config.getBkgType();
    switch (bkgType)
    {
        case BkgParams::BkgType::chev:
        b = new Chevychev2(var,"bkg",config.getInitValues()->getChk4_k1(),config.getInitValues()->getChk4_k2());
        break;

        case BkgParams::BkgType::special:
        b = new SpecialBkg(var,"bkg",config.getInitValues()->getMu(),
                        config.getInitValues()->getSigmaBkg(),config.getInitValues()->getLambda());
        break;

        case BkgParams::BkgType::exponential:
        b = new ExponentialBkg(var,"bkg",config.getInitValues()->getLambda());

        break;

        case BkgParams::BkgType::none:
        b = new BkgFunc();
        break;

        default:
        throw std::invalid_argument("\nError: bkgType argument not valid\n");
    }
    return b;
}