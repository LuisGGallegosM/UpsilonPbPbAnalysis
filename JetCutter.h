#pragma once

#include "JetSkimmer.h"

class JetCutter
{
    private:

    public:
    bool operator()(Jet_Input* input, Int_t index);
    JetCutter();
};