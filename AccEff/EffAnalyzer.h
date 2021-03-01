#ifndef EFFANALYZER
#define EFFANALYZER

#include "TH2.h"
#include "TEfficiency.h"

#include "../TreeProcessor/TreeProcessor.h"
#include "../OniaBase/OniaWriter.h"
#include "../OniaBase/OniaReader.h"
#include "AccEffCutter.h"
#include "../Skimming/OniaCutter.h"

class EffAnalyzer : public TreeProcessor
{
    private:
    TH2F* etaVsPtQQRecoCut;
    TH2F* etaVsPtMuRecoCut;
    TH1F* ptHistQQRecoCut;
    TH1F* ptHistQQDet;
    TEfficiency* ptQQEfficiency;

    std::unique_ptr<OniaReader> oniaReader;
    std::unique_ptr<EffCutter> effCutter;
    std::unique_ptr<OniaWriter> oniaWriter;

    void Analyze(Int_t index, Long64_t entry);

    public:
    EffAnalyzer(OniaReader* input,EffCutter* effCut, OniaWriter* writer, TH1F* QQDetHist );

    void Write(const std::string& basename);

    void Test() { Process(oniaReader.get()); }
    void ProcessEvent(Long64_t entry) override;
};


#endif