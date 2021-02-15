#ifndef ACCEFFINPUT
#define ACCEFFINPUT

#include "TClonesArray.h"
#include <unordered_map>

#define maxBranchSize (1000)

struct AccEffInput
{
    TClonesArray* mom4_RecoQQ;
    TClonesArray* mom4_RecoMu;
    TClonesArray* mom4_GenQQ;
    TClonesArray* mom4_GenMu;
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

    AccEffInput();
    ~AccEffInput();
};

#endif