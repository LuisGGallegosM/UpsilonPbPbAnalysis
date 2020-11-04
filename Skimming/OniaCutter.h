#ifndef ONIACUTTER
#define ONIACUTTER

#include "HLT_triggers.h"
#include "OniaSkimmer.h"
#include "../Params.h"

class OniaCutter
{
    private:
    
    cutParams kineCut;

    bool isSoft(Onia_Input* input, Int_t index);

    public:
    bool operator()(Onia_Input* input, Int_t index,Int_t entry);
    OniaCutter(const cutParams* cut);
};

#endif