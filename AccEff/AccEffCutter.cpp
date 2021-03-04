#include "AccEffCutter.h"

bool AccCutter::cut(const OniaReader* input,Int_t index,Int_t entry)
{
    if(!isMuonInAcceptance((TLorentzVector*) input->genQQ_mupl_mom4->At(index))) return false;
    if(!isMuonInAcceptance((TLorentzVector*) input->genQQ_mumi_mom4->At(index))) return false;

    return true;
}


bool AccCutter::cut2(const OniaReader* input,Int_t index,Int_t entry)
{
    int mupl_idx = input->genQQ_mupl_idx[index];//plus muon index
    int mumi_idx = input->genQQ_mumi_idx[index];//minus muon index
    if(!isMuonInAcceptance((TLorentzVector*) input->genMu_mom4->At(mupl_idx))) return false;
    if(!isMuonInAcceptance((TLorentzVector*) input->genMu_mom4->At(mumi_idx))) return false;

    return true;
}

bool AccCutter::isMuonInAcceptance(const TLorentzVector* Muon) const
{
    //Acceptance cuts in CMS AN-18-316 , Page 17
    float eta= fabs(Muon->Eta());
    float pt = Muon->Pt();
    if (eta> 2.4f) return false;                        // 0.0 < eta < 2.4
    if (eta< 1.2f)  return (pt >  3.5f);                // 0.0 < eta < 1.2
    else if (eta<2.1f) return (pt > (5.47f-1.89f*eta)); // 1.2 < eta < 2.1
    else return pt >1.5f;                               // 2.1 < eta < 2.4
}

EffCutter::EffCutter(const CutParams* cut) : kineCut(*cut)
{

}

bool EffCutter::cut(const OniaReader* input,Int_t index,Int_t entry)
{
    int mupl_idx = input->recoQQ_mupl_idx[index];//plus muon index
    int mumi_idx = input->recoQQ_mumi_idx[index];//minus muon index

    //are both muons reconstructed?
    if (input->RecoMuWhichGen[mupl_idx] < 0) return false;
    if (input->RecoMuWhichGen[mumi_idx] < 0) return false;

    int genQQIndex=input->RecoQQWhichGen[index];
    if (genQQIndex < 0) return false;

    TLorentzVector* mom4=(TLorentzVector*)input->recoQQ_mom4->At(index);
    if ((mom4->M() < 8.5f) || (mom4->M() >11.0f)) return false;

    return kineCut.cut(input,index,entry);
}