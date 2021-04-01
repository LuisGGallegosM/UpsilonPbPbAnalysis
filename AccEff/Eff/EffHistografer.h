#ifndef EFFHISTOGRAFER
#define EFFHISTOGRAFER

#include "TH2.h"
#include "TEfficiency.h"
#include "TLorentzVector.h"

class EffHistografer
{
    TH2F* etaVsPtQQRecoCut;
    TH2F* etaVsPtMuRecoCut;
    TH2F* etaVsPtQQDet;
    TH1F* ptHistQQRecoCut;
    TH1F* ptHistQQDet;
    std::unique_ptr<TEfficiency> ptQQEfficiency;
    std::unique_ptr<TEfficiency> etaVsPtQQEfficiency;

    public:
    struct inputs
    {
        float pT;
        float y;
        float etaMuPl;
        float etaMuMi;
        float ptMuPl;
        float ptMuMi;
    };
    EffHistografer();

    void FillDet(float y, float pT);
    void FillRecoCut(const inputs* input);
    void Write(const std::string& basename);
};

template<typename Data>
EffHistografer::inputs extractRecoCut( const Data* input ,int index)
{
    EffHistografer::inputs data;

    TLorentzVector* mom4vec=(TLorentzVector*) input->recoQQ.mom4->At(index);

    int mupl_idx = input->recoQQ.mupl_idx[index];//plus muon index
    int mumi_idx = input->recoQQ.mumi_idx[index];//minus muon index
    TLorentzVector* mom4vecPl=(TLorentzVector*) input->recoMu.mom4->At(mupl_idx);
    TLorentzVector* mom4vecMi=(TLorentzVector*) input->recoMu.mom4->At(mumi_idx);

    data.pT = mom4vec->Pt();
    data.y = fabs(mom4vec->Rapidity());
    data.etaMuPl=fabs(mom4vecPl->Eta());
    data.etaMuMi=fabs(mom4vecMi->Eta());
    data.ptMuPl=mom4vecPl->Pt();
    data.ptMuMi=mom4vecMi->Pt();
    return data;
}

#endif