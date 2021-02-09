#ifndef ONIACUTTER
#define ONIACUTTER

#include "OniaData.h"
#include "Cutter.h"
#include "HLT_triggers.h"
#include "../Utils/Params.h"

class OniaCutter : public Cutter<Onia_Input>
{
    private:
    cutParams kineCut;

    bool isSoft(Onia_Input* input, Int_t index);
    Int_t findMatchGenQQ(Int_t genMuPl, Int_t genMuMi, Int_t* genQQpl, Int_t* genQQmi, int size);

    public:
    bool cut(Onia_Input* input, Int_t index,Int_t entry) override;
    bool prescale(Int_t entry) override;
    bool isMC() override { return kineCut.isMC; }

    OniaCutter(const cutParams* cut);
};

#if defined(__CLING__)
#include "OniaCutter.cpp"
#endif

#endif