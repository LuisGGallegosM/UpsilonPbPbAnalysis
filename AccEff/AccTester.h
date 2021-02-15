#ifndef ACCTESTER
#define ACCTESTER

#include "../TreeProcessor/TreeProcessor.h"
#include "../TreeProcessor/Outputer.h"
#include "AccEffCutter.h"
#include "AccEffInput.h"
#include "TH2.h"

class AccOutputer : public TreeOutputer
{
    private:
    TH2F* etaVsPtHist;
    TH1F* ptHist;
    TH1F* ptAccHist;
    std::map<int,int> pdgIds;
    Int_t Evt;
    Float_t mass;
    Float_t pT;
    Float_t y;
    Float_t phi;
    Float_t eta;
    Float_t dR;
    Int_t pdgId;

    public:
    AccOutputer(const char* treeOutName);

    void WriteData(const AccEffInput& dataIn,Int_t index, Long64_t entry);
    void Write() override;
};

class AccTester : public TreeProcessor
{
    private:
    AccEffInput dataIn;
    AccEffCutter* cutter;
    AccOutputer* outputer;
    void ProcessEvent(Long64_t entry) override;

    public:
    AccTester(TTree* treeIn,AccOutputer* outp , AccEffCutter* cut);
    void Test() { Process(); }
};

#endif