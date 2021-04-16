#include<iostream>

#include"../Fit/Fit.h"
#include"../../Utils/Tester/Tester.h"

using std::cout;
using std::string;

void testFitFunctions(Tester* t)
{
    ParameterGroup p;
    std::unique_ptr<BkgFunc> bkg;
    RooRealVar mass("mass","mass",0.0);
    bkg.reset( BkgFactory(mass,p) );
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

