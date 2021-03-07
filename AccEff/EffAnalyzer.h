#ifndef EFFANALYZER
#define EFFANALYZER

#include "TH2.h"
#include "TEfficiency.h"

#include "../TreeProcessor/TreeProcessor.h"
#include "../OniaBase/OniaWriter.h"
#include "../OniaBase/OniaReader.h"
#include "AccEffCutter.h"
#include "../Skimming/OniaCutter.h"

constexpr const char* effDenName= "p_{t} QQ Detectable Efficiency";
constexpr const char* effNumName= "p_{t} QQ Reco + Cut";

class EffAnalyzer : public TreeProcessor
{
    private:
    TH2F* etaVsPtQQRecoCut;
    TH2F* etaVsPtMuRecoCut;
    TH2F* etaVsPtQQDet;
    TH1F* ptHistQQRecoCut;
    TH1F* ptHistQQDet;
    std::unique_ptr<TEfficiency> ptQQEfficiency;
    std::unique_ptr<TEfficiency> etaVsPtQQEfficiency;

    std::unique_ptr<OniaReader> oniaReader;
    std::unique_ptr<EffCutter> effCutter;
    std::unique_ptr<OniaWriter> oniaWriter;
    std::unique_ptr<AccCutter> accCutter;

    void Analyze(Int_t index, Long64_t entry);
    void CaptureRecoQQ(Int_t index, Long64_t entry);
    void CaptureDetQQ(Int_t index, Long64_t entry);

    public:
    EffAnalyzer(OniaReader* input,EffCutter* effCut, OniaWriter* writer, AccCutter* accCut );

    void Write(const std::string& basename);

    void Test() { Process(oniaReader->getReader()); }
    void ProcessEvent(Long64_t entry) override;
};


#endif