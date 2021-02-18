#ifndef ACCTESTER
#define ACCTESTER

#include "../TreeProcessor/TreeProcessor.h"
#include "AccOutputer.h"
#include "AccEffCutter.h"
#include "AccEffInput.h"

class AccTester : public TreeProcessor
{
    private:
    AccEffInput dataIn;
    AccEffCutter* cutter;
    AccOutputer* outputer;
    void ProcessEvent(Long64_t entry) override;

    public:
    AccTester(TTree* treeIn,AccOutputer* outp , AccEffCutter* cut);
    void Test() { Process(); }
};

#endif