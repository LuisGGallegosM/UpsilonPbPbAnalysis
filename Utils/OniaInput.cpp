
#include "OniaInput.h"

OniaInput::baseQQ::baseQQ()
{
    mom4 = new TClonesArray("TLorentzVector");
    mupl_idx = new Int_t[maxBranchSize];
    mumi_idx = new Int_t[maxBranchSize];
    sign = new Int_t[maxBranchSize];
}
OniaInput::baseQQ::~baseQQ()
{
    delete mom4;
    delete[] mupl_idx;
    delete[] mumi_idx;
    delete[] sign;
}

OniaInput::baseMu::baseMu()
{
    mom4 = new TClonesArray("TLorentzVector");
}

OniaInput::baseMu::~baseMu()
{
    delete mom4;
}

OniaInput::OniaInput() : genQQ(), recoQQ(), genMu(), recoMu()
{
    SelectionType = new Int_t[maxBranchSize];
    nTrkWMea = new Int_t[maxBranchSize];
    nPixWMea = new Int_t[maxBranchSize];
    dxy = new Float_t[maxBranchSize];
    dz = new Float_t[maxBranchSize]; 
    VtxProb = new Float_t[maxBranchSize];
    trig = new ULong64_t[maxBranchSize];
    RecoMuWhichGen = new Int_t[maxBranchSize];
    GenMuWhichReco = new Int_t[maxBranchSize];
    GenQQWhichReco = new Int_t[maxBranchSize];
    GenQQid = new Int_t[maxBranchSize];
}

OniaInput::~OniaInput()
{
    delete[] SelectionType;
    delete[] nTrkWMea;
    delete[] nPixWMea;
    delete[] dxy;
    delete[] dz;
    delete[] VtxProb;
    delete[] trig;
    delete[] RecoMuWhichGen;
    delete[] GenMuWhichReco;
    delete[] GenQQid;
}