#ifndef ONIAOUTPUT
#define ONIAOUTPUT

#include "RtypesCore.h"
#include "Skimmer.h"
#include "OniaData.h"

struct Onia_Output_QQ
{
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
};

struct Onia_Output_Mu
{
    Int_t Evt;
    Float_t mass;
    Float_t pT;
    Float_t y;
    Float_t phi;
    Float_t eta;
    Float_t dR;
};

struct Onia_Aux
{
    std::unordered_map<Long64_t,Onia_Output_QQ> events;
};

class OniaOutputer
{
    public:
    virtual void setOutputs(Skimmer& skimmer) = 0;
    virtual void WriteData(const Onia_Input& dataIn,Int_t index, Long64_t entry) = 0;
    virtual ~OniaOutputer() {};
};

class OniaOutputerQQ : public OniaOutputer
{
    private:
    Onia_Output_QQ dataOut;
    public:
    void setOutputs(Skimmer& skimmer) override;
    void WriteData(const Onia_Input& dataIn,Int_t index, Long64_t entry) override;

    OniaOutputerQQ() : dataOut() {}
};

class OniaOutputerMu : public OniaOutputer
{
    private:
    Onia_Output_Mu dataOut;
    public:
    void setOutputs(Skimmer& skimmer) override;
    void WriteData(const Onia_Input& dataIn,Int_t index, Long64_t entry) override;

    OniaOutputerMu() : dataOut() {}
};

#endif