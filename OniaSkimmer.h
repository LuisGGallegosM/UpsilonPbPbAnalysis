
#pragma once

#include "TTree.h"
#include "TLorentzVector.h"
#include "TString.h"
#include "TClonesArray.h"
#include <new>
#include <functional>

#define maxBranchSize (1000)
#define MAXTREESIZE (100000000)

struct Onia_Input
{
    TClonesArray* mom4;
    Int_t size;
    Int_t* mupl_idx;
    Int_t* mumi_idx;
    Int_t* SelectionType;
    Int_t* nTrkWMea;
    Int_t* nPixWMea;
    Float_t* dxy;
    Float_t* dz;
    Float_t* VtxProb;
    ULong64_t* trig;
    UInt_t runNb;
    UInt_t eventNb;
    UInt_t lumi;

    Onia_Input();
    ~Onia_Input();
};

struct Onia_Output
{
    Float_t mass;
    Float_t pT;
    Float_t y;
    Float_t phi;
    Float_t eta;
};

class OniaSkimmer
{
    private:
    std::vector<TBranch*> input_branches;
    Onia_Input oniaDataIn;
    Onia_Output oniaDataOut;
    TTree* tree;
    TTree* tree_output;

    void InitBranches();
    void GetEntries(long index);
    void WriteData(long index);
    
    public:
    OniaSkimmer(TTree* myTree);
    void Skim(std::function<bool(Onia_Input*,long)> cutter);
    TTree* GetTree();
};

