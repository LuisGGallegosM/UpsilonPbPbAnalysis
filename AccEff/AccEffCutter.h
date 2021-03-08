
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
    template<typename Reader>
    bool cut(const Reader* input, Int_t index,Int_t entry);
};

class EffCutter
{
    private:
    CutParams kineCut;

    public:
    template<typename Reader>
    bool cut(const Reader* input, Int_t index,Int_t entry) ;
    
    EffCutter(const CutParams* cut)  : kineCut(*cut) {}
};

#if defined(__CLING__)
#include "OniaCutter.cpp"
#endif

#endif