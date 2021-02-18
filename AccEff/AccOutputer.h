#ifndef ACCOUTPUTER
#define ACCOUTPUTER

#include "TH2.h"
#include "TEfficiency.h"

#include "../TreeProcessor/Outputer.h"
#include "AccEffInput.h"
#include "AccEffCutter.h"

class AccOutputer : public TreeOutputer
{
    private:
    TH2F* etaVsPtQQ;
    TH2F* etaVsPtQQCut;
    TH2F* etaVsPtMu;
    TH2F* etaVsPtMuCut;
    TH1F* ptHist;
    TH1F* ptHistCut;
    TEfficiency* ptAccHist;
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
    AccOutputer(const char* treeOutName);

    void WriteData(const AccEffInput& dataIn,Int_t index, Long64_t entry,AccEffCutter& cut);
    void Write(const std::string& basename) override;
};


#endif