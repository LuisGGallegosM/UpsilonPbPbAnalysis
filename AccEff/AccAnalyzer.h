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

constexpr const char* accDenName ="pt QQ Generated";
constexpr const char* accNumName ="pt QQ Detectable Acceptancy";

class AccAnalyzer : public TreeProcessor
{
    private:
    TH2F* etaVsPtQQGen;
    TH2F* etaVsPtQQDet;
    TH2F* etaVsPtMuGen;
    TH2F* etaVsPtMuDet;
    TH1F* ptHistQQGen;
    TH1F* ptHistQQDet;
    std::unique_ptr<TEfficiency> ptQQAcceptancy;
    std::unique_ptr<TEfficiency> etaVsPtQQAcceptancy;

    OniaReaderGenOnly oniaReader;
    AccCutter accCutter;
    OniaWriterGenOnly oniaWriter;

    void Analyze(Int_t index, Long64_t entry);

    public:
    AccAnalyzer(TTree* input,const char* outTreeName);

    void Write(const std::string& basename);

    void Test() { Process(oniaReader.getReader()); }
    void ProcessEvent(Long64_t entry) override;

    TH1F* getPtHistQQDet() {return ptHistQQDet;}
};


#endif