#ifndef ACCTESTER
#define ACCTESTER

#include "../TreeProcessor/TreeProcessor.h"
#include "AccEffOutputer.h"
#include "AccEffCutter.h"
#include "../Utils/OniaInput.h"

class AccEffTester : public TreeProcessor
{
    private:
    OniaInput dataIn;
    AccCutter* accCutter;
    EffCutter* effCutter;
    AccEffOutputer* outputer;
    void ProcessEvent(Long64_t entry) override;

    public:
    AccEffTester(TTree* treeIn,AccEffOutputer* outp , AccCutter* accCut, EffCutter* effCut);
    void Test() { Process(); }
};

#endif