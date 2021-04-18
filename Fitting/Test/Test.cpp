#include<iostream>

#include"../Fit/Fit.h"
#include"../../Utils/Tester/Tester.h"

using std::cout;
using std::string;

void testFitFunctions(Tester* t)
{
    ParameterGroup p;
    p.deserialize("../HelperScripts/examplefiles/exFitMC.fit");

    const ParameterGroup* bkgParam=p.get("bkg");
    auto varNames= bkgParam->getVarNames();
    auto subgNames= bkgParam->getSubgroupNames();

    std::unique_ptr<BkgFunc> bkg;
    RooRealVar mass("mass","mass",0.0);
    bkg.reset( BkgFactory(mass,*bkgParam) );

    auto pOutput= bkg->getBkgParams();
    for ( const auto& var : varNames)
        t->assert_true(pOutput.exists(var), var+" variable retrieved correctly");
    for ( const auto& subg : subgNames)
        t->assert_true(pOutput.exists(subg), subg+" subgroup retrieved correctly");
}

void testFit(Tester* t)
{
    t->test(testFitFunctions,"FitFunctions");
}

int main()
{
    Tester test("Fitting test");
    testFit(&test);
    return 0;
}

