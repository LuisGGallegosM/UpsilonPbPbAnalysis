#ifndef EFFANALYZER
#define EFFANALYZER

#include "TH2.h"
#include "TEfficiency.h"

#include "../TreeProcessor/TreeProcessor.h"
#include "../OniaBase/OniaIO/OniaIO.h"
#include "AccEffCutter.h"

constexpr const char* effDenName= "p_{t} QQ Detectable Efficiency";
constexpr const char* effNumName= "p_{t} QQ Reco + Cut";

class EffAnalyzer
{
    public:
    virtual void Write(const std::string& basename) = 0;
    virtual void Test() = 0;

    virtual ~EffAnalyzer() = default;
};

template<typename Reader>
class EffAnalyzerBase : public TreeProcessor, public EffAnalyzer
{
    private:
    TH2F* etaVsPtQQRecoCut;
    TH2F* etaVsPtMuRecoCut;
    TH2F* etaVsPtQQDet;
    TH1F* ptHistQQRecoCut;
    TH1F* ptHistQQDet;
    std::unique_ptr<TEfficiency> ptQQEfficiency;
    std::unique_ptr<TEfficiency> etaVsPtQQEfficiency;

    OniaReader<Reader> oniaReader;
    EffCutter effCutter;
    OniaWriterRecoQQ oniaWriter;
    std::unique_ptr<AccCutter> accCutter;

    void Analyze(Int_t index, Long64_t entry);
    void CaptureRecoQQ(Int_t index, Long64_t entry);
    void CaptureDetQQ(Int_t index, Long64_t entry);

    public:
    EffAnalyzerBase(TTree* input,CutParams* effCut, const char* outTreeName );

    void Write(const std::string& basename) override;

    void Test() override { Process(); }
    void ProcessEvent(Long64_t entry) override;
};

using EffAnalyzerRealData = EffAnalyzerBase<OniaRealData>;
using EffAnalyzerMC = EffAnalyzerBase<OniaMCData>;

std::unique_ptr<EffAnalyzer> createEffAnalyzer(TTree* input,CutParams* effCut, const char* outTreeName );

#endif