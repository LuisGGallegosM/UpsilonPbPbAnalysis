#ifndef SKIMMER
#define SKIMMER

#include "TTree.h"
#include "TString.h"
#include "TClonesArray.h"
#include <iostream>

#define MAXTREESIZE (25000000000)

class Skimmer
{
    private:
    std::vector<TBranch*> input_branches;
    TTree* tree_input;
    void GetEntries(Long64_t index);

    protected:
    virtual void ProcessEvent(Long64_t entry) = 0;
    
    void addInput(const char* varName,void* address);

    public:
    Skimmer(TTree* treeIn);
    virtual ~Skimmer() {}

    void Skim();
};

#endif