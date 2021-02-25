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
    AccEffOutputer* outputer;
    void ProcessEvent(Long64_t entry) override;

    public:
    AccEffTester(TTree* treeIn,AccEffOutputer* outp);
    void Test() { Process(); }
};

#endif