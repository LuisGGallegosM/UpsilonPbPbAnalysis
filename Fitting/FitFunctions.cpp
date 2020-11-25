

#include"FitFunctions.h"

//Crystal ball member functions.

CrystalBall::CrystalBall(RooRealVar& var, const char* name, const dcbParam* initial):
    mean(   Form("mean_%s",name),"mean of gaussian PDF",initial->getMean(),S1_MEAN_MIN,S1_MEAN_MAX),
    sigma(  Form("sigma_%s",name),"width of gaussian",initial->getSigma(), S1_SIGMA_MIN, S1_SIGMA_MAX),
    alpha(  Form("alpha_%s",name),"tail shift", initial->getAlpha(),S1_ALPHA_MIN,S1_ALPHA_MAX),
    n(      Form("n_%s",name),"power order",initial->getN(), S1_N_MIN, S1_N_MAX),
    cBall(  Form("cball_%s",name),"crystalBall",var,mean,sigma,alpha,n)
{

}

RooCBShape* CrystalBall::getCB()
{
    return &cBall;
}

//Double CrystalBall

DoubleCrystalBall::DoubleCrystalBall(RooRealVar& var,const char* name, const dcbParam* initial):
    CrystalBall(var,Form("%s_1",name),initial),
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

void DoubleCrystalBall::getFitParams(dcbParam* params)
{
    params->setParams(mean.getVal(),alpha.getVal(),n.getVal(),sigma.getVal(),x.getVal(),f.getVal());
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

//Chevychev2

Chevychev2::Chevychev2(RooRealVar& var,const char* name,float k1,float k2):
    ch4_k1("chk4_k1","chk4_k1",k1,S1_CH4_MIN,S1_CH4_MAX),
    ch4_k2("chk4_k2","chk4_k2",k2,S1_CH4_MIN,S1_CH4_MAX),
    chev(name,"Background",var,RooArgSet(ch4_k1,ch4_k2))
{

}

//SpecialBkg

SpecialBkg::SpecialBkg(RooRealVar& var,const char* name,float mu_,float sigma_, float lambda_):
    mu(Form("mu_%s",name),"err_mu",mu_,  0, 35),
    sigma(Form("sigma_%s",name),"err_sigma", sigma_, 0,35),
    lambda(Form("lambda_%s",name),"m_lambda",  lambda_, 0,35),
    bkgPdf()
{
    RooGenericPdf* pdf=
        new RooGenericPdf(name,"Background","TMath::Exp(-@0/@1)*(TMath::Erf((@0-@2)/(TMath::Sqrt(2)*@3))+1)*0.5",
                           RooArgList(var, lambda, mu, sigma));
    bkgPdf.reset(pdf);
}

    