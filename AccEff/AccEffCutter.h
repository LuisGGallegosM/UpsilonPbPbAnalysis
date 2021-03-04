
#ifndef ACCEFFCUTTER
#define ACCEFFCUTTER

#include "TLorentzVector.h"
#include "../OniaBase/OniaReader.h"
#include "../OniaBase/CutParams.h"

class AccCutter
{
    private:
    bool isMuonInAcceptance(const TLorentzVector* Muon) const;

    public:
    bool cut(const OniaReader* input, Int_t index,Int_t entry);
    bool cut2(const OniaReader* input, Int_t index,Int_t entry);
    bool prescale(Int_t entry) const { return false; }

    std::string getName() const {return "Acceptancy test";};
};

class EffCutter
{
    private:
    CutParams kineCut;

    public:
    bool cut(const OniaReader* input, Int_t index,Int_t entry) ;
    bool prescale(Int_t entry) const { return false; }

    std::string getName() const {return "Efficiency test";};
    
    EffCutter(const CutParams* cut);
};

#if defined(__CLING__)
#include "OniaCutter.cpp"
#endif

#endif