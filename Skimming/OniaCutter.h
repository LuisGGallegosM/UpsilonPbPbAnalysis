#ifndef ONIACUTTER
#define ONIACUTTER

#include "HLT_triggers.h"
#include "OniaSkimmer.h"

class OniaCutter
{
    private:
    Int_t selectionBits;
    Int_t minTracks;
    Int_t minPixels;
    Float_t maxDxy;
    Float_t maxDz;
    Float_t minVtxProb;
    bool isMC;
    bool checkSign;
    ULong64_t trigSelect;
    Int_t sign;

    bool isSoft(Onia_Input* input, Int_t index);

    public:
    bool operator()(Onia_Input* input, Int_t index);
    OniaCutter();
};

#endif