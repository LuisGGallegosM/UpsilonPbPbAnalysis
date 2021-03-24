
#ifndef EFFCUTTER
#define EFFCUTTER

#include "TLorentzVector.h"

#include "../../OniaBase/OniaIO/OniaIO.h"
#include "../../OniaBase/Params/Params.h"
#include "../Common/AccCutter.h"

class EffCutter
{
    private:
    CutParams kineCut;

    public:
    template<typename Reader>
    bool cut(const Reader* input, Int_t index,Int_t entry)
    {
        int mupl_idx = input->recoQQ.mupl_idx[index];//plus muon indexGenOnly
        int mumi_idx = input->recoQQ.mumi_idx[index];//minus muon indexGenOnly
        if (input->which.RecoMuWhichGen[mupl_idx] < 0) return false;
        if (input->which.RecoMuWhichGen[mumi_idx] < 0) return false;

        int genQQIndex=input->which.RecoQQWhichGen[index];
        if (genQQIndex < 0) return false;

        TLorentzVector* mom4=(TLorentzVector*)input->recoQQ.mom4->At(index);
        if ((mom4->M() < 8.5f) || (mom4->M() >11.0f)) return false;

        return kineCut.cut(input,index,entry);
    }
    
    EffCutter(const CutParams* cut)  : kineCut(*cut) {}
};

#if defined(__CLING__)
#include "OniaCutter.cpp"
#endif

#endif