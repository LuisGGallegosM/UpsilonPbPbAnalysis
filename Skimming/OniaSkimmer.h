
#ifndef ONIASKIMMER
#define ONIASKIMMER

#include "Skimmer.h"
#include "OniaCutter.h"
#include "OniaOutput.h"

class OniaSkimmer : public Skimmer
{
    Onia_Input dataIn;
    protected:
    OniaCutter* cutter;
    OniaOutputer* outputer;
    void ProcessEvent(Long64_t entry) override;
    public:
    OniaSkimmer(TTree* treeIn,OniaOutputer* outp , OniaCutter* cut);
};

#if defined(__CLING__)
#include "OniaSkimmer.cpp"
#endif

#endif