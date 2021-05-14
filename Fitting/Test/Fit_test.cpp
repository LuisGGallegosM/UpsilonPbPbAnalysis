
#include"../Fit/Fit.h"
#include"../../Utils/Tester/Tester.h"

using std::cout;
using std::string;

const std::string exPath="../HelperScripts/examplefiles";

void testParams(Tester* t, const ParameterGroup* p1, const ParameterGroup* p2)
{
    auto varNames= p1->getVarNames();
    auto subgNames= p1->getSubgroupNames();
    for ( const auto& var : varNames)
        t->assert_true(p2->exists(var), var+" variable retrieved correctly");
    for ( const auto& subg : subgNames)
        t->assert_true(p1->exists(subg), subg+" subgroup retrieved correctly");
}

void testFitFunctionsBkg(Tester* t)
{
    ParameterGroup p;
    p.deserialize(exPath+"/exFitMC.fitconf");

    const ParameterGroup* bkgParam=p.get("bkg");
    std::unique_ptr<FitFunc> bkg;
    RooRealVar mass("mass","mass",0.0);
    bkg.reset( BkgFactory(mass,bkgParam) );

    auto pOutput= bkg->getBkgParams();

    testParams(t,bkgParam,&pOutput);
}

void testFitFunctionsDCB(Tester* t)
{
    ParameterGroup p;
    p.deserialize(exPath+"/exFitMC.fitconf");

    const ParameterGroup* dcbParams= p.get("signal");

    RooRealVar mass("mass","mass",0.0);
    DoubleCrystalBall dcb(mass,"dcb",dcbParams);

    auto dcbOut= dcb.getParams();

    testParams(t,dcbParams,&dcbOut);
}

void testOniaMassFit(Tester* t, const std::string& file)
{
    ParameterGroup p;
    p.deserialize(file);
    auto massFitter = createMassFitter(nullptr,&p);

    const ParameterGroup out= massFitter->getFitParams();

    testParams(t,&p,&out);

    getKineCutExpr(&p);

}

void testFit(Tester* t)
{
    t->test(testFitFunctionsBkg,"FitFunctions bkg");
    t->test(testFitFunctionsDCB,"FitFunctions dcb");
    t->test( [](Tester* t){testOniaMassFit(t,exPath+"/exFitMC.fitconf");},"oniaMassFit MC");
    t->test( [](Tester* t){testOniaMassFit(t,exPath+"/exFitDATA.fitconf");},"oniaMassFit DATA");
}