#ifndef ACCHISTOGRAFER
#define ACCHISTOGRAFER

#include"TH2.h"
#include"TEfficiency.h"

class AccHistografer
{
    public:
    TH2F* etaVsPtQQGen;
    TH2F* etaVsPtQQDet;
    TH2F* etaVsPtMuGen;
    TH2F* etaVsPtMuDet;
    TH1F* ptHistQQGen;
    TH1F* ptHistQQDet;
    std::unique_ptr<TEfficiency> ptQQAcceptancy;
    std::unique_ptr<TEfficiency> etaVsPtQQAcceptancy;

    struct inputs
    {
        float pT;
        float y;
        float etaMuPl;
        float etaMuMi;
        float ptMuPl;
        float ptMuMi;
    };

    AccHistografer();
    void FillGen(const inputs* in,float weight=1.0f);
    void FillDet(const inputs* in,float weight=1.0f);
    void finalCalculations(const std::string& basename);
    void Write() const;
};

#endif