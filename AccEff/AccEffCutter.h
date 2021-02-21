
#ifndef ACCEFFCUTTER
#define ACCEFFCUTTER

#include "TLorentzVector.h"
#include "../Utils/OniaInput.h"
#include "../TreeProcessor/TreeCutter.h"
#include "../Utils/SkimParams.h"

class AccCutter : public TreeCutter<OniaInput>
{
    private:

    public:
    bool cut(const OniaInput* input, Int_t index,Int_t entry) override;
    bool prescale(Int_t entry) const override { return false; }
    bool isMuonInAcceptance(const TLorentzVector* Muon) const;

    std::string getName() const override {return "Acceptancy test";};
    
    AccCutter() {}
    virtual ~AccCutter()=default;
};

class EffCutter : public TreeCutter<OniaInput>
{
    private:
    cutParams kineCut;

    public:
    bool cut(const OniaInput* input, Int_t index,Int_t entry) override;
    bool prescale(Int_t entry) const override { return false; }

    std::string getName() const override {return "Efficiency test";};
    
    EffCutter(const cutParams* cut);
    virtual ~EffCutter()=default;
};

#if defined(__CLING__)
#include "OniaCutter.cpp"
#endif

#endif