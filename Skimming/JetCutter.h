#ifndef JETCUTTER
#define JETCUTTER

#include "JetSkimmer.h"

class JetCutter : public Cutter<Jet_Input>
{
    private:
    Onia_Aux* auxData;
    Float_t minPt;
    Float_t maxPt;

    public:
    bool cut(Jet_Input* input, Int_t index, Int_t entry) override;
    bool prescale(Int_t entry) override;
    JetCutter(Onia_Aux* auxDat);
};

#endif