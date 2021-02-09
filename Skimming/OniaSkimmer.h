
#ifndef ONIASKIMMER
#define ONIASKIMMER

#include "Skimmer.h"
#include "OniaCutter.h"

class OniaSkimmer : public Skimmer<Onia_Input,Onia_Output>
{
    private:
    void ProcessEvent(Long64_t entry) override;
    OniaCutter* cutter;
    public:
    OniaSkimmer(TTree* treeIn,const char* treeOutName, OniaCutter* cut);

    void WriteData(Int_t index, Long64_t entry);
    std::unique_ptr<Onia_Aux> auxData;
};

#if defined(__CLING__)
#include "OniaSkimmer.cpp"
#endif

#endif