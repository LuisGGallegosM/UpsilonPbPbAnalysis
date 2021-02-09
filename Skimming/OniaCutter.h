#ifndef ONIACUTTER
#define ONIACUTTER

#include "OniaData.h"
#include "Cutter.h"
#include "HLT_triggers.h"
#include "../Utils/Params.h"

class OniaCutter : public Cutter<Onia_Input>
{
    public:
    enum class loopObj { none, recoQQ, recoMu, genQQ, genMu };
    virtual loopObj getLoopObj() const = 0;
};

class OniaCutterRecoQQ : public OniaCutter
{
    private:
    cutParams kineCut;
    bool isSoft(Onia_Input* input, Int_t index) const;
    Int_t findMatchGenQQ(Int_t genMuPl, Int_t genMuMi, Int_t* genQQpl, Int_t* genQQmi, int size) const;

    public:
    bool cut(Onia_Input* input, Int_t index,Int_t entry) override;
    bool prescale(Int_t entry) const override {return (kineCut.prescale>1) && ((entry % kineCut.prescale)!=0);}
    bool isMC() const override { return kineCut.isMC; }
    loopObj getLoopObj() const override { return loopObj::recoQQ;};
    
    OniaCutterRecoQQ(const cutParams* cut);
};

class OniaCutterRecoMu : public OniaCutter
{
    public:
    bool cut(Onia_Input* input, Int_t index,Int_t entry) override;
    bool prescale(Int_t entry) const override {return false;}
    bool isMC() const override { return true; }
    loopObj getLoopObj() const override { return loopObj::recoMu;};
};

#if defined(__CLING__)
#include "OniaCutter.cpp"
#endif

#endif