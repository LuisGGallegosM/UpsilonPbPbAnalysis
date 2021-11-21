

#include"FitFunctions.h"
#include"../Common/Common.h"

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

ParameterGroup CrystalBall::getParams() const
{
    ParameterGroup p;
    ParameterWrite(p,mean,"mean");
    ParameterWrite(p,sigma,"sigma");
    ParameterWrite(p,alpha,"alpha");
    ParameterWrite(p,n,"n");
    return p;
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
    x.setConstant(g->getBool("x.fixed"));
    f.setConstant(g->getBool("f.fixed"));
}

ParameterGroup DoubleCrystalBall::getParams() const
{
    ParameterGroup p;
    ParameterWrite(p,x,"x");
    ParameterWrite(p,f,"f");
    p.setString("type","dcb");
    p.addGroup( CrystalBall::getParams() );
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

//crystalballexp

CrystalBallGauss::CrystalBallGauss(RooRealVar& var,const char* name, const ParameterGroup* g):
    CrystalBall(var,Form("%s_1",name),g),
    mean_2(   Form("mean_%s_2",name), "1.0*@0",RooArgList(mean)),
    x(        Form("x_%s",name),"sigma ratio",g->getFloat("x.value"),g->getFloat("x.low"),g->getFloat("x.high")),
    sigma_2(  Form("sigma_%s_2",name),"@0*@1",RooArgList(x,sigma)),
    f(        Form("f_%s",name),     "Crystal ball ratio", g->getFloat("f.value"),g->getFloat("f.low"),g->getFloat("f.high")),
    gauss(  Form("cball_%s_2",name),"gaussian",var,mean_2,sigma_2),
    cbexpball(   Form("dcb_%s",name),    "crystal ball and exp", RooArgList(cBall,gauss),RooArgList(f) )
{
    x.setConstant(g->getBool("x.fixed"));
    f.setConstant(g->getBool("f.fixed"));
}

ParameterGroup CrystalBallGauss::getParams() const
{
    ParameterGroup p;
    ParameterWrite(p,x,"x");
    ParameterWrite(p,f,"f");
    p.setString("type","cbgauss");
    p.addGroup( CrystalBall::getParams() );
    return p;
}

CrystalGaussSlave::CrystalGaussSlave(RooRealVar& var,const char* name,CrystalBallGauss& doublecb,float ratio):
    CrystalBallSlave(var,doublecb,name,ratio),
    mean_2(   Form("mean_%s_2",name),   "1.0*@0",RooArgList(mean)),
    x(        Form("x_%s",name),        "1.0*@0",RooArgList(doublecb.x)),
    sigma_2(  Form("sigma_%s_2",name),  "@0*@1", RooArgList(doublecb.x,sigma)),
    f(        Form("f_%s",name),        "1.0*@0",RooArgList(doublecb.f)),
    gauss(  Form("cball_%s_2",name),  "gaussian",var,mean_2,sigma_2),
    dcball(   Form("dcb_%s",name),      "crystal ball and gauss", RooArgList(cBall,gauss),RooArgList(f) )
{

}

//NoFitFunc

RooAbsReal* NoFitFunc::getFunc()
{ 
    throw std::runtime_error("Error: accesing no bkg funcion"); 
}

ParameterGroup NoFitFunc::getBkgParams() const
{
    ParameterGroup p;
    p.setString("type","none");
    return p;
}

//Chevychev

std::vector<RooRealVar> produceChevVars(const ParameterGroup* init)
{
    std::vector<RooRealVar> chk;
    std::vector<std::string> varNames;
    
    int i=1;
    while ( init->exists("chk4_k"+std::to_string(i)) && (i<20))
    {
        varNames.push_back("chk4_k"+std::to_string(i));
        i++;
    };

    chk.reserve(varNames.size());

    for (auto& name : varNames)
    {
        chk.push_back( RooRealVar(name.data(),name.data(),getParamVal(*init,name),getParamLow(*init,name),getParamHigh(*init,name)) );
    }
    return chk;
}

RooArgSet produceChevArgSet(const std::vector<RooRealVar>& chk)
{
    RooArgSet set;
    for(auto& v : chk)
    {
        set.add(v);
    }
    return set;
}

RooArgList produceChevArgList(const std::vector<RooRealVar>& chk,const RooRealVar& var)
{
    RooArgList set;
    set.add(var);
    for(auto& v : chk)
    {
        set.add(v);
    }
    return set;
}

Chevychev::Chevychev(RooRealVar& var,const char* name,const ParameterGroup* init):
    chk(produceChevVars(init)),
    chev(name,"Background",var,produceChevArgSet(chk))
{
    
}

ParameterGroup Chevychev::getBkgParams() const
{
    ParameterGroup p;
    int i=0;
    for(auto& v : chk)
    {
        ParameterWrite(p,v,v.GetName());
        i++;
    }
    p.setString("type","chev");
    return p;
}

std::string getChebPoly(int order)
{
    std::string str;
    switch(order)
    {
        case 1:
        str="TMath::Exp(@0*@1)";
        break;
        case 2:
        str="TMath::Exp(@2*(2.0*@0*@0-1.0)+@1*@0+1.0)";
        break;
    }
    return str;
}

//ExpChevycheb
ExpChevychev::ExpChevychev(RooRealVar& var,const char* name,const ParameterGroup* init):
    chk(produceChevVars(init)),
    expr(getChebPoly(chk.size())),
    expo(name,"Background",expr.data(),produceChevArgList(chk,var))
{
    
}

ParameterGroup ExpChevychev::getBkgParams() const
{
    ParameterGroup p;
    int i=0;
    for(auto& v : chk)
    {
        ParameterWrite(p,v,v.GetName());
        i++;
    }
    p.setString("type", "expchev");
    return p;
}

//SpecialBkg

SpecialBkg::SpecialBkg(RooRealVar& var,const char* name,const ParameterGroup* init):
    mu(Form("mu_%s",name),"err_mu",getParamVal(*init,"mu"),  getParamLow(*init,"mu"), getParamHigh(*init,"mu")),
    sigma(Form("sigma_%s",name),"err_sigma",getParamVal(*init,"sigma"), getParamLow(*init,"sigma"),getParamHigh(*init,"sigma")),
    lambda(Form("lambda_%s",name),"m_lambda",getParamVal(*init,"lambda"),getParamLow(*init,"lambda"),getParamHigh(*init,"lambda")),
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

ExponentialBkg::ExponentialBkg(RooRealVar& var,const char* name,const ParameterGroup* init):
lambda(Form("lambda_%s",name),"m_lambda",getParamVal(*init,"lambda"),  getParamLow(*init,"lambda"), getParamHigh(*init,"lambda")),
bkgPdf()
{
    RooGenericPdf* pdf=
        new RooGenericPdf(name,"Background","TMath::Exp(@0*@1)",
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

FitFunc* BkgFactory(RooRealVar& var, const ParameterGroup* config)
{
    FitFunc* b= nullptr;
    BkgType bkgType = getBkgByName(config->getString("type"));
    switch (bkgType)
    {
        case BkgType::chev:
        b = new Chevychev(var,bkgName,config);
        break;

        case BkgType::expchev:
        b = new ExpChevychev(var,bkgName,config);
        break;

        case BkgType::special:
        b = new SpecialBkg(var,bkgName,config);
        break;

        case BkgType::exponential:
        b = new ExponentialBkg(var,bkgName,config);
        break;

        case BkgType::none:
        b = new NoFitFunc();
        break;

        default:
        throw std::invalid_argument("\nError: bkgType argument not valid\n");
    }
    return b;
}