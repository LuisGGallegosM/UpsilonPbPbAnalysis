#ifndef JETCUTTER
#define JETCUTTER

#include "JetSkimmer.h"
#include "OniaSkimmer.h"

class JetCutter
{
    private:
    Onia_Aux* auxData;
    Float_t minPt;
    Float_t maxPt;

    public:
    bool operator()(Jet_Input* input, Int_t index);
    JetCutter(Onia_Aux* auxDat);
};

#endif