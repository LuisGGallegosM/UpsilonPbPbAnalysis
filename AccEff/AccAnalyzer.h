#ifndef ACCANALYZER
#define ACCANALYZER

#include "TH2.h"
#include "TEfficiency.h"

#include "../TreeProcessor/TreeProcessor.h"
#include "../OniaBase/OniaWriter.h"
#include "../OniaBase/OniaReader.h"
#include "AccEffCutter.h"
#include "../Skimming/OniaCutter.h"
#include "AccEffAux.h"

class AccAnalyzer : public TreeProcessor
{
    private:
    TH2F* etaVsPtQQGen;
    TH2F* etaVsPtQQDet;
    TH2F* etaVsPtMuGen;
    TH2F* etaVsPtMuDet;
    TH1F* ptHistQQGen;
    TH1F* ptHistQQDet;
    TEfficiency* ptQQAcceptancy;

    std::unique_ptr<OniaReader> oniaReader;
    std::unique_ptr<AccCutter> accCutter;
    std::unique_ptr<OniaWriter> oniaWriter;

    void Analyze(Int_t index, Long64_t entry);

    public:
    AccAnalyzer(OniaReader* input,AccCutter* accCut, OniaWriter* writer);

    void Write(const std::string& basename);

    void Test() { Process(oniaReader.get()); }
    void ProcessEvent(Long64_t entry) override;

    TH1F* getPtHistQQDet() {return ptHistQQDet;}
};


#endif