#ifndef ONIACUTTER
#define ONIACUTTER

#include "../OniaBase/OniaReader.h"
#include "../OniaBase/CutParams.h"

class OniaCutter
{
    private:
    CutParams kineCut;
    bool isSoft(const OniaReader* input, Int_t index) const;

    public:
    bool cut(const OniaReader* input, Int_t index,Int_t entry) {return kineCut.cut(input,index,entry);}
    bool prescale(Int_t entry) const {return kineCut.isPrescaled(entry); }
    bool isMC() const { return kineCut.getIsMC(); }
    std::string getName() const {return "Onia reco QQ";};
    
    OniaCutter(const CutParams* cut);
};

#if defined(__CLING__)
#include "OniaCutter.cpp"
#endif

#endif