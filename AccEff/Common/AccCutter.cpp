
#include"AccCutter.h"

bool AccCutter::cut(const OniaGenOnlyData* input,Int_t index,Int_t entry)
{
    if(!isMuonInAcceptance((TLorentzVector*) input->genQQ.mupl_mom4->At(index))) return false;
    if(!isMuonInAcceptance((TLorentzVector*) input->genQQ.mumi_mom4->At(index))) return false;

    return true;
}

bool isMuonInAcceptance(const TLorentzVector* Muon)
{
    //Acceptance cuts in CMS AN-18-316 , Page 17
    float eta= fabs(Muon->Eta());
    float pt = Muon->Pt();
    if (eta> 2.4f) return false;                        // 0.0 < eta < 2.4
    if (eta< 1.2f)  return (pt >  3.5f);                // 0.0 < eta < 1.2
    else if (eta<2.1f) return (pt > (5.47f-1.89f*eta)); // 1.2 < eta < 2.1
    else return pt >1.5f;                               // 2.1 < eta < 2.4
}