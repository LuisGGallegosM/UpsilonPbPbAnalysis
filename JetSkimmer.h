#pragma once

#include "Skimmer.h"
#include "OniaSkimmer.h"

struct Jet_Input
{
    Int_t evt;
    Int_t nRef;
    Float_t* rawPt;
    Float_t* jtPt;
    Float_t* jtEta;
    Float_t* jtPhi;
    Float_t* jtY;
    Float_t* jtPu;
    Float_t* jtM;
    Float_t* jtArea;

    Int_t getSize();

    Jet_Input();
    ~Jet_Input();
};

struct Jet_Output
{
    Int_t evt;
    Float_t rawPt;
    Float_t jtPt;
    Float_t jtEta;
    Float_t jtPhi;
    Float_t jtY;
    Float_t jtPu;
    Float_t jtM;
    Float_t jtArea;
    Float_t z;
};

class JetSkimmer : public Skimmer<Jet_Input,Jet_Output>
{
    Onia_Aux* oniaData;

    void WriteData(Int_t index, Long64_t entry);
    public:
    JetSkimmer(TTree* treeIn,const char* treeOutName, Onia_Aux* auxData);
};