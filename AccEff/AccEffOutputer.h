#ifndef ACCEFFOUTPUTER
#define ACCEFFOUTPUTER

#include "TH2.h"
#include "TEfficiency.h"

#include "../TreeProcessor/Outputer.h"
#include "../Utils/OniaInput.h"
#include "AccEffCutter.h"
#include "../Skimming/OniaCutter.h"

class AccEffOutputer : public TreeOutputer
{
    private:
    TH2F* etaVsPtQQ;
    TH2F* etaVsPtQQAcc;
    TH2F* etaVsPtQQEff;
    TH2F* etaVsPtMu;
    TH2F* etaVsPtMuAcc;
    TH2F* etaVsPtMuEff;
    TH1F* ptHist;
    TH1F* ptHistAcc;
    TH1F* ptHistEff;
    TEfficiency* ptAcceptancy;
    TEfficiency* ptEfficiency;
    Int_t Evt;
    Float_t mass;
    Float_t pT;
    Float_t y;
    Float_t phi;
    Float_t eta;
    Float_t dR;
    Int_t pdgId;

    TH2F* createTH2(const std::string& name,const std::string& title);
    TH1F* createTH1(const std::string& name,const std::string& title);

    void writeToCanvas(TH1* hist,const std::string& xname,const std::string& yname, const std::string& outname);
    void writeToCanvasEff(TEfficiency* hist,const std::string& xname,const std::string& yname, const std::string& outname);

    public:
    AccEffOutputer(const char* treeOutName);

    void WriteData(const OniaInput& dataIn,Int_t index, Long64_t entry,AccCutter& accCut,EffCutter& effCut);
    void Write(const std::string& basename) override;
};


#endif