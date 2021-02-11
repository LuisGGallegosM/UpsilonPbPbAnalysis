
#ifndef ONIASKIMMER
#define ONIASKIMMER

#include "../TreeProcessor/TreeProcessor.h"
#include "OniaCutter.h"
#include "OniaOutput.h"

class OniaSkimmer : public TreeProcessor
{
    private:
    Onia_Input dataIn;
    OniaCutter* cutter;
    OniaOutputer* outputer;
    void ProcessEvent(Long64_t entry) override;

    public:
    OniaSkimmer(TTree* treeIn,OniaOutputer* outp , OniaCutter* cut);
    void Skim() { Process(); }
};

#if defined(__CLING__)
#include "OniaSkimmer.cpp"
#endif

#endif