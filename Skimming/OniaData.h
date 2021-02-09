
#include "TClonesArray.h"
#include <unordered_map>

#define maxBranchSize (1000)

struct Onia_Input
{
    TClonesArray* mom4_QQ;
    TClonesArray* mom4_mu;
    Int_t recoQQsize;
    Int_t genQQsize;
    Int_t recoMuSize;
    Int_t genMuSize;
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
    Int_t* RecoMuWhichGen;
    
    Int_t* genQQ_mupl_idx;
    Int_t* genQQ_mumi_idx;
    Int_t* GenQQid;
    Int_t* sign;

    Int_t getSizeRecoQQ() const {return recoQQsize;}
    Int_t getSizeGenQQ() const {return genQQsize;}
    Int_t getSizeRecoMu() const {return recoMuSize;}
    Int_t getSizeGenMu() const {return genMuSize;}

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