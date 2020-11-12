

#include"FitFunctions.h"

//Crystal ball member functions.

CrystalBall::CrystalBall(RooRealVar& var, const char* name, const dcbParam* initial):
    mean(   Form("mean_%s",name),"mean of gaussian PDF",initial->mean,S1_MEAN_MIN,S1_MEAN_MAX),
    sigma(  Form("sigma_%s",name),"width of gaussian",initial->sigma1, S1_SIGMA_MIN, S1_SIGMA_MAX),
    alpha(  Form("alpha_%s",name),"tail shift", initial->alpha,S1_ALPHA_MIN,S1_ALPHA_MAX),
    n(      Form("n_%s",name),"power order",initial->n, S1_N_MIN, S1_N_MAX),
    cBall(  Form("cball_%s",name),"crystalBall",var,mean,sigma,alpha,n)
{

}

RooCBShape* CrystalBall::getCB()
{
    return &cBall;
}

//Chevychev2

Chevychev2::Chevychev2(RooRealVar& var,const char* name,float k1,float k2):
    ch4_k1("chk4_k1","chk4_k1",k1,S1_CH4_MIN,S1_CH4_MAX),
    ch4_k2("chk4_k2","chk4_k2",k2,S1_CH4_MIN,S1_CH4_MAX),
    chev(name,"Background",var,RooArgSet(ch4_k1,ch4_k2))
{

}

RooChebychev* Chevychev2::getChev()
{
    return &chev;
}

//Double CrystalBall

DoubleCrystalBall::DoubleCrystalBall(RooRealVar& var, const dcbParam* initial):
    CrystalBall(var,"1",initial),
    mean_2(   "mean_2", "1.0*@0",RooArgList(mean)),
    x(        "x","sigma ratio",initial->x,0.0f,1.0f),
    sigma_2(  "sigma_2","@0*@1",RooArgList(x,sigma)),
    alpha_2(  "alpha_2","1.0*@0",RooArgList(alpha)),
    n_2(      "n_2",    "1.0*@0",RooArgList(n)),
    f(       "f",     "Crystal ball ratio", initial->f, 0.0f, 1.0f),
    cBall_2(  "cball_2","crystalBall",var,mean_2,sigma_2,alpha_2,n_2),
    dcball(   "dcb",    "double crystal ball", RooArgList(cBall,cBall_2),RooArgList(f) )
{

}

RooAbsPdf* DoubleCrystalBall::getDCB()
{
    return &dcball;
}