#ifndef ACCCUTTER
#define ACCCUTTER

#include "TLorentzVector.h"

#include "../../OniaBase/OniaIO/OniaIO.h"

bool isMuonInAcceptance(const TLorentzVector* Muon);

class AccCutter
{
    public:
    template<typename Reader>
    bool cut(const Reader* input, Int_t index,Int_t entry)
    {
        int mupl_idx = input->genQQ.mupl_idx[index];//plus muon index
        int mumi_idx = input->genQQ.mumi_idx[index];//minus muon index
        if(!isMuonInAcceptance((TLorentzVector*) input->genMu.mom4->At(mupl_idx))) return false;
        if(!isMuonInAcceptance((TLorentzVector*) input->genMu.mom4->At(mumi_idx))) return false;

        return true;
    }

    bool cut(const OniaGenOnlyData* input,Int_t index,Int_t entry);
};


#endif