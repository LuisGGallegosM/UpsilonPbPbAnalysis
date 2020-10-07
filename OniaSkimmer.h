
#pragma once

#include "Skimmer.h"

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
    Int_t* whichGen;

    Int_t getSize();

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

class OniaSkimmer : public Skimmer<Onia_Input,Onia_Output>
{
    private:
    void WriteData(Int_t index);
    public:
    OniaSkimmer(TTree* myTree);
};

