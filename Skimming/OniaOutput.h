#ifndef ONIAOUTPUT
#define ONIAOUTPUT

#include "TTree.h"
#include "../OniaBase/OniaInput.h"
#include "TH1.h"
#include "../TreeProcessor/Outputer.h"

class OniaOutputer : public TreeOutputer
{
    public:
    OniaOutputer(const char* treeOutName) : TreeOutputer(treeOutName) { }
    virtual void WriteData(const OniaInput& dataIn,Int_t index, Long64_t entry) = 0;
};

class OniaOutputerQQ : public OniaOutputer
{
    private:
    Int_t Evt;
    Float_t mass;
    Float_t pT;
    Float_t y;
    Float_t phi;
    Float_t eta;
    Float_t pT_mi;
    Float_t eta_mi;
    Float_t phi_mi;
    Float_t pT_pl;
    Float_t eta_pl;
    Float_t phi_pl;

    public:
    OniaOutputerQQ(const char* treeOutName);

    void WriteData(const OniaInput& dataIn,Int_t index, Long64_t entry) override;
};

#endif