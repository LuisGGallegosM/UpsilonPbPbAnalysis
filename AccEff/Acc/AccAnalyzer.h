#ifndef ACCANALYZER
#define ACCANALYZER

#include "TH2.h"
#include "TEfficiency.h"
#include "RooAbsReal.h"

#include "../../OniaBase/TreeProcessor/TreeProcessor.h"
#include "../../OniaBase/OniaIO/OniaIO.h"
#include "../Common/AccEffAux.h"
#include "../Common/AccCutter.h"

class AccHistografer
{
    TH2F* etaVsPtQQGen;
    TH2F* etaVsPtQQDet;
    TH2F* etaVsPtMuGen;
    TH2F* etaVsPtMuDet;
    TH1F* ptHistQQGen;
    TH1F* ptHistQQDet;
    std::unique_ptr<TEfficiency> ptQQAcceptancy;
    std::unique_ptr<TEfficiency> etaVsPtQQAcceptancy;

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

    AccHistografer();
    void FillGen(const inputs* in,float weight=1.0f);
    void FillDet(const inputs* in,float weight=1.0f);
    void Write(const std::string& basename);
};

class AccAnalyzer : public TreeProcessor
{
    private:
    AccHistografer hists;
    OniaReader<OniaGenOnlyData> oniaReader;
    AccCutter accCutter;
    OniaWriterGenQQ oniaWriter;
    RooAbsReal* weightFunc;

    void Analyze(Int_t index, Long64_t entry);

    public:
    AccAnalyzer(TTree* input,const char* outTreeName, RooAbsReal* weightFunc=nullptr);

    void Write(const std::string& basename);

    void Test() { Process(); }
    void ProcessEvent(Long64_t entry) override;
};

#endif