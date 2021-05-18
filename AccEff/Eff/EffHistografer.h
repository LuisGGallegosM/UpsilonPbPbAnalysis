#ifndef EFFHISTOGRAFER
#define EFFHISTOGRAFER

#include "TH2.h"
#include "TEfficiency.h"
#include "TLorentzVector.h"

class EffHistografer
{
    public:
    TH2F* etaVsPtQQRecoCut;
    TH2F* etaVsPtMuRecoCut;
    TH2F* etaVsPtQQDet;
    TH1F* ptHistQQRecoCut;
    TH1F* ptHistQQDet;
    std::unique_ptr<TEfficiency> ptQQEfficiency;
    std::unique_ptr<TEfficiency> etaVsPtQQEfficiency;

    struct inputs
    {
        float pT;
        float y;
        float etaMuPl;
        float etaMuMi;
        float ptMuPl;
        float ptMuMi;
        float yMuPl;
        float yMuMi;
    };
    EffHistografer();

    void finalCalculations(const std::string& basename);

    void FillDet(float y, float pT, float weight=1.0f);
    void FillRecoCut(const inputs* input, float weight=1.0f);
    void Write() const;
};

template<typename Data>
EffHistografer::inputs extractGen( const Data* input ,int index)
{
    EffHistografer::inputs data;

    TLorentzVector* mom4vec=(TLorentzVector*) input->genQQ.mom4->At(index);

    int mupl_idx = input->genQQ.mupl_idx[index];//plus muon index
    int mumi_idx = input->genQQ.mumi_idx[index];//minus muon index
    TLorentzVector* mom4vecPl=(TLorentzVector*) input->genMu.mom4->At(mupl_idx);
    TLorentzVector* mom4vecMi=(TLorentzVector*) input->genMu.mom4->At(mumi_idx);

    data.pT = mom4vec->Pt();
    data.y = fabs(mom4vec->Rapidity());
    data.etaMuPl=fabs(mom4vecPl->Eta());
    data.etaMuMi=fabs(mom4vecMi->Eta());
    data.ptMuPl=mom4vecPl->Pt();
    data.ptMuMi=mom4vecMi->Pt();
    data.yMuPl=mom4vecPl->Rapidity();
    data.yMuMi=mom4vecMi->Rapidity();
    return data;
}

#endif