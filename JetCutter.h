#pragma once

#include "JetSkimmer.h"
#include "OniaSkimmer.h"

class JetCutter
{
    private:
    Onia_Aux* auxData;

    public:
    bool operator()(Jet_Input* input, Int_t index);
    JetCutter(Onia_Aux* auxDat);
};