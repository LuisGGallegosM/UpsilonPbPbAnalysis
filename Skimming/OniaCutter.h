#ifndef ONIACUTTER
#define ONIACUTTER

#include "OniaData.h"
#include "../TreeProcessor/TreeCutter.h"
#include "HLT_triggers.h"
#include "../Utils/SkimParams.h"

class OniaCutter : public TreeCutter<Onia_Input>
{
    public:
    virtual bool isMC() const =0;
    virtual ~OniaCutter() {}
};

class OniaCutterRecoQQ : public OniaCutter
{
    private:
    cutParams kineCut;
    bool isSoft(const Onia_Input* input, Int_t index) const;

    public:
    bool cut(const Onia_Input* input, Int_t index,Int_t entry) override;
    bool prescale(Int_t entry) const override { return (kineCut.prescale>1) && ((entry % kineCut.prescale)!=0); }
    bool isMC() const override { return kineCut.isMC; }
    std::string getName() const override {return "Onia reco QQ";};
    
    OniaCutterRecoQQ(const cutParams* cut);
};

#if defined(__CLING__)
#include "OniaCutter.cpp"
#endif

#endif