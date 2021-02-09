
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
    OniaSkimmer(TTree* treeIn,const char* treeOutName, OniaOutputer* outp , OniaCutter* cut);
    //std::unique_ptr<Onia_Aux> auxData;
};

#if defined(__CLING__)
#include "OniaSkimmer.cpp"
#endif

#endif