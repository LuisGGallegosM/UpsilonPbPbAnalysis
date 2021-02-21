
#include "OniaInput.h"

OniaInput::OniaInput()
{
    mom4_RecoQQ = new TClonesArray("TLorentzVector");
    mom4_RecoMu = new TClonesArray("TLorentzVector");
    mom4_GenQQ = new TClonesArray("TLorentzVector");
    mom4_GenMu = new TClonesArray("TLorentzVector");
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
    GenMuWhichReco = new Int_t[maxBranchSize];
    genQQ_mupl_idx = new Int_t[maxBranchSize];
    genQQ_mumi_idx = new Int_t[maxBranchSize];
    GenQQid = new Int_t[maxBranchSize];
    sign = new Int_t[maxBranchSize];
}

OniaInput::~OniaInput()
{
    delete mom4_RecoQQ;
    delete mom4_RecoMu;
    delete mom4_GenQQ;
    delete mom4_GenMu;
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
    delete[] GenMuWhichReco;
    delete[] genQQ_mupl_idx;
    delete[] genQQ_mumi_idx;
    delete[] GenQQid;
    delete[] sign;
}

Int_t OniaInput::findRecoParent(int recoMuPl,int recoMuMi) const
{
    Int_t parentIndex=-1;
    for(int i=0;i< recoMuSize;i++)
    {
        if ((mupl_idx[i]==recoMuPl) && (mumi_idx[i]==recoMuMi) )
        {
            parentIndex=i;
            break;
        }
    }
    return parentIndex;
}