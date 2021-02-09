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
    std::vector<TBranch*> output_branches;
    TTree* tree_input;
    TTree* tree_output;

    void GetEntries(Long64_t index);

    protected:
    virtual void ProcessEvent(Long64_t entry) = 0;
    void FillEntries();

    public:
    Skimmer(TTree* treeIn, const char* treeOutName);
    virtual ~Skimmer() {}

    TTree* Skim();
    TTree* GetTree() {return tree_output;}
    void addOutput(const char* varName,Float_t* var);
    void addOutput(const char* varName,Int_t* var);
    void addInput(const char* varName,void* address);
};

#endif