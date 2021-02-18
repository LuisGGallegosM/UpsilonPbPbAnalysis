#include "AccEffCutter.h"

bool AccCutter::cut(const OniaInput* input,Int_t index,Int_t entry)
{
    int mupl_idx = input->genQQ_mupl_idx[index];//plus muon index
    int mumi_idx = input->genQQ_mumi_idx[index];//minus muon index

    if(!isMuonInAcceptance((TLorentzVector*) input->mom4_GenMu->At(mupl_idx))) return false;
    if(!isMuonInAcceptance((TLorentzVector*) input->mom4_GenMu->At(mumi_idx))) return false;

    return true;
}

bool AccCutter::isMuonInAcceptance(const TLorentzVector* Muon) const
{
    //Acceptance cuts in CMS AN-18-316 , Page 17
    float eta= fabs(Muon->Eta());
    float pt = Muon->Pt();
    if(eta>2.4f) return false;                          // 0.0 < eta < 2.4
    if(pt < 3.5f) return false;

    if      (eta<1.2f) return (pt >  3.5f);             // 0.0 < eta < 1.2
    else if (eta<2.1f) return (pt > (5.47f-1.89f*eta)); // 2.1 < eta < 2.4
    else return pt >1.5f;                               // 1.2 < eta < 2.1
}

EffCutter::EffCutter(const cutParams* cut) : kineCut(*cut)
{

}

bool EffCutter::cut(const OniaInput* input,Int_t index,Int_t entry)
{
    return true;
}