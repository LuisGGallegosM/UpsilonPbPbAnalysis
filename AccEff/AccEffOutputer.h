#ifndef ACCEFFOUTPUTER
#define ACCEFFOUTPUTER

#include "TH2.h"
#include "TEfficiency.h"

#include "../TreeProcessor/Outputer.h"
#include "../OniaBase/OniaInput.h"
#include "AccEffCutter.h"
#include "../Skimming/OniaCutter.h"

class AccEffOutputer : public TreeOutputer
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
    Int_t Evt;
    Float_t mass;
    Float_t pT;
    Float_t y;
    Float_t phi;
    Float_t eta;
    Float_t dR;
    Int_t pdgId;

    AccCutter* accCutter;
    EffCutter* effCutter;

    TH2F* createTH2QQ(const std::string& name,const std::string& title);
    TH2F* createTH2Mu(const std::string& name,const std::string& title);
    TH1F* createTH1(const std::string& name,const std::string& title);

    void writeToCanvasBase(TH1* hist,const std::string& xname,const std::string& yname, const std::string& outname, const std::string& option);
    void writeToCanvas(TH1* hist,const std::string& xname,const std::string& yname, const std::string& outname);
    void writeToCanvas(TH2* hist,const std::string& xname,const std::string& yname, const std::string& outname);
    void writeToCanvasEff(TEfficiency* hist,const std::string& xname,const std::string& yname, const std::string& outname);

    public:
    AccEffOutputer(const char* treeOutName,AccCutter* accCut, EffCutter* effCut);

    void WriteData(const OniaInput& dataIn,Int_t index, Long64_t entry);
    void Write(const std::string& basename) override;
};


#endif