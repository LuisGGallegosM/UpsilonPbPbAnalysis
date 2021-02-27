
#ifndef ACCEFFCUTTER
#define ACCEFFCUTTER

#include "TLorentzVector.h"
#include "../OniaBase/OniaReader.h"
#include "../TreeProcessor/TreeCutter.h"
#include "../OniaBase/SkimParams.h"

class AccCutter : public TreeCutter<OniaReader>
{
    private:
    bool isMuonInAcceptance(const TLorentzVector* Muon) const;

    public:
    bool cut(const OniaReader* input, Int_t index,Int_t entry) override;
    bool prescale(Int_t entry) const override { return false; }

    std::string getName() const override {return "Acceptancy test";};
    
    AccCutter() {}
    virtual ~AccCutter()=default;
};

class EffCutter : public TreeCutter<OniaReader>
{
    private:
    cutParams kineCut;

    public:
    bool cut(const OniaReader* input, Int_t index,Int_t entry) override;
    bool prescale(Int_t entry) const override { return false; }

    std::string getName() const override {return "Efficiency test";};
    
    EffCutter(const cutParams* cut);
    virtual ~EffCutter()=default;
};

#if defined(__CLING__)
#include "OniaCutter.cpp"
#endif

#endif