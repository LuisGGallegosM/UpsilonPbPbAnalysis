#pragma once

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
    ULong64_t trigSelect;

    bool isSoft(Onia_Input* input, long index);

    public:
    bool operator()(Onia_Input* input, long index);
    OniaCutter();
};