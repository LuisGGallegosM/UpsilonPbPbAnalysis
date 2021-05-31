#ifndef ACCHISTOGRAFER
#define ACCHISTOGRAFER

#include"TH2.h"
#include"TEfficiency.h"

class AccHistografer
{
    public:
    TH2D* etaVsPtQQGen;
    TH2D* etaVsPtQQDet;
    TH2D* etaVsPtMuGen;
    TH2D* etaVsPtMuDet;
    TH1D* ptHistQQGen;
    TH1D* ptHistQQDet;
    std::unique_ptr<TEfficiency> ptQQAcceptancy;
    std::unique_ptr<TEfficiency> etaVsPtQQAcceptancy;

    struct inputs
    {
        double pT;
        double y;
        double etaMuPl;
        double etaMuMi;
        double ptMuPl;
        double ptMuMi;
    };

    AccHistografer();
    void FillGen(const inputs* in,double weight=1.0);
    void FillDet(const inputs* in,double weight=1.0);
    void finalCalculations(const std::string& basename);
    void Write() const;
};

#endif