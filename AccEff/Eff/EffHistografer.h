#ifndef EFFHISTOGRAFER
#define EFFHISTOGRAFER

#include "TH2.h"
#include "TEfficiency.h"
#include "TLorentzVector.h"

class EffHistografer
{
    public:
    TH2D* etaVsPtQQRecoCut;
    TH2D* etaVsPtMuRecoCut;
    TH2D* etaVsPtQQDet;
    TH1D* ptHistQQRecoCut;
    TH1D* ptHistQQDet;
    std::unique_ptr<TEfficiency> ptQQEfficiency;
    std::unique_ptr<TEfficiency> etaVsPtQQEfficiency;

    struct inputs
    {
        double pT;
        double y;
        double etaMuPl;
        double etaMuMi;
        double ptMuPl;
        double ptMuMi;
        double yMuPl;
        double yMuMi;
    };
    EffHistografer();

    void finalCalculations(const std::string& basename);

    void FillDet(double y, double pT, double weight=1.0);
    void FillRecoCut(const inputs* input, double weight=1.0);
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
    data.y = abs(mom4vec->Rapidity());
    data.etaMuPl=abs(mom4vecPl->Eta());
    data.etaMuMi=abs(mom4vecMi->Eta());
    data.ptMuPl=mom4vecPl->Pt();
    data.ptMuMi=mom4vecMi->Pt();
    data.yMuPl=mom4vecPl->Rapidity();
    data.yMuMi=mom4vecMi->Rapidity();
    return data;
}

#endif