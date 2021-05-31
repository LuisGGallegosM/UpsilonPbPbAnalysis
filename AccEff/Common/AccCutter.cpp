
#include"AccCutter.h"

bool AccCutter::cut(const OniaGenOnlyData* input,Int_t index,Int_t entry)
{
    if(!isMuonInAcceptance((TLorentzVector*) input->genQQ.mupl_mom4->At(index))) return false;
    if(!isMuonInAcceptance((TLorentzVector*) input->genQQ.mumi_mom4->At(index))) return false;

    return true;
}

bool isMuonInAcceptance(const TLorentzVector* Muon)
{
    return (Muon->Pt() > 3.5) && ( abs(Muon->Eta()) < 2.4);
}