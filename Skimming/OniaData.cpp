
#include "OniaData.h"

Onia_Input::Onia_Input()
{
    mom4_QQ = new TClonesArray("TLorentzVector");
    mom4_mu = new TClonesArray("TLorentzVector");
    mupl_idx = new Int_t[maxBranchSize];
    mumi_idx = new Int_t[maxBranchSize];
    SelectionType = new Int_t[maxBranchSize];
    nTrkWMea = new Int_t[maxBranchSize];
    nPixWMea = new Int_t[maxBranchSize];
    dxy = new Float_t[maxBranchSize];
    dz = new Float_t[maxBranchSize]; 
    VtxProb = new Float_t[maxBranchSize];
    trig = new ULong64_t[maxBranchSize];
    RecoMuWhichGen = new Int_t[maxBranchSize];
    genQQ_mupl_idx = new Int_t[maxBranchSize];
    genQQ_mumi_idx = new Int_t[maxBranchSize];
    GenQQid = new Int_t[maxBranchSize];
    sign = new Int_t[maxBranchSize];
}

Onia_Input::~Onia_Input()
{
    delete mom4_QQ;
    delete mom4_mu;
    delete[] mupl_idx;
    delete[] mumi_idx;
    delete[] SelectionType;
    delete[] nTrkWMea;
    delete[] nPixWMea;
    delete[] dxy;
    delete[] dz;
    delete[] VtxProb;
    delete[] trig;
    delete[] RecoMuWhichGen;
    delete[] genQQ_mupl_idx;
    delete[] genQQ_mumi_idx;
    delete[] GenQQid;
    delete[] sign;
}