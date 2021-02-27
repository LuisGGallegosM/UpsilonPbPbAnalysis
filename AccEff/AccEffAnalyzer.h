#ifndef ACCEFFANALYZER
#define ACCEFFANALYZER

#include "TH2.h"
#include "TEfficiency.h"

#include "../TreeProcessor/TreeProcessor.h"
#include "../OniaBase/OniaWriter.h"
#include "../OniaBase/OniaReader.h"
#include "AccEffCutter.h"
#include "../Skimming/OniaCutter.h"

class AccEffAnalyzer : public TreeProcessor
{
    private:
    TH2F* etaVsPtQQGen;
    TH2F* etaVsPtQQDet;
    TH2F* etaVsPtQQRecoCut;
    TH2F* etaVsPtMuGen;
    TH2F* etaVsPtMuDet;
    TH2F* etaVsPtMuRecoCut;
    TH1F* ptHistQQGen;
    TH1F* ptHistQQDet;
    TH1F* ptHistQQRecoCut;
    TEfficiency* ptQQAcceptancy;
    TEfficiency* ptQQEfficiency;

    std::unique_ptr<OniaReader> oniaReader;
    std::unique_ptr<AccCutter> accCutter;
    std::unique_ptr<EffCutter> effCutter;
    std::unique_ptr<OniaWriter> oniaWriter;

    TH2F* createTH2QQ(const std::string& name,const std::string& title);
    TH2F* createTH2Mu(const std::string& name,const std::string& title);
    TH1F* createTH1(const std::string& name,const std::string& title);

    void writeToCanvasBase(TH1* hist,const std::string& xname,const std::string& yname, const std::string& outname, const std::string& option);
    void writeToCanvas(TH1* hist,const std::string& xname,const std::string& yname, const std::string& outname);
    void writeToCanvas(TH2* hist,const std::string& xname,const std::string& yname, const std::string& outname);
    void writeToCanvasEff(TEfficiency* hist,const std::string& xname,const std::string& yname, const std::string& outname);

    public:
    AccEffAnalyzer(OniaReader* input,AccCutter* accCut, EffCutter* effCut, OniaWriter* writer);

    void WriteData(Int_t index, Long64_t entry);

    void Write(const std::string& basename);

    void Test() { Process(oniaReader.get()); }
    void ProcessEvent(Long64_t entry) override;
};


#endif