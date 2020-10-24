
#ifndef ONIASKIMMER
#define ONIASKIMMER

#include <unordered_map>
#include <memory>
#include "Skimmer.h"

struct Onia_Input
{
    TClonesArray* mom4_QQ;
    TClonesArray* mom4_mu;
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
    Int_t* sign;

    Int_t getSize();

    Onia_Input();
    ~Onia_Input();
};

struct Onia_Output
{
    Int_t Evt;
    Float_t mass;
    Float_t pT;
    Float_t y;
    Float_t phi;
    Float_t eta;
    Float_t pT_mi;
    Float_t eta_mi;
    Float_t phi_mi;
    Float_t pT_pl;
    Float_t eta_pl;
    Float_t phi_pl;
};

struct Onia_Aux
{
    std::unordered_map<Long64_t,Onia_Output> events;
};

class OniaSkimmer : public Skimmer<Onia_Input,Onia_Output>
{
    private:
    void WriteData(Int_t index, Long64_t entry);
    public:
    OniaSkimmer(TTree* treeIn,const char* treeOutName);

    std::unique_ptr<Onia_Aux> auxData;
};

#endif