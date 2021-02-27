#ifndef ONIACUTTER
#define ONIACUTTER

#include "../OniaBase/OniaReader.h"
#include "../TreeProcessor/TreeCutter.h"
#include "HLT_triggers.h"
#include "../OniaBase/SkimParams.h"

class OniaCutter : public TreeCutter<OniaReader>
{
    public:
    virtual bool isMC() const =0;
    virtual ~OniaCutter()=default;
};

class OniaCutterRecoQQ : public OniaCutter
{
    private:
    cutParams kineCut;
    bool isSoft(const OniaReader* input, Int_t index) const;

    public:
    bool cut(const OniaReader* input, Int_t index,Int_t entry) override {return kineCut.cut(input,index,entry);}
    bool prescale(Int_t entry) const override {return kineCut.isPrescaled(entry); }
    bool isMC() const override { return kineCut.getIsMC(); }
    std::string getName() const override {return "Onia reco QQ";};
    
    OniaCutterRecoQQ(const cutParams* cut);
    virtual ~OniaCutterRecoQQ()=default;
};

#if defined(__CLING__)
#include "OniaCutter.cpp"
#endif

#endif