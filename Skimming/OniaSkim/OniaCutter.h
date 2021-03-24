#ifndef ONIACUTTER
#define ONIACUTTER

#include "../../OniaBase/OniaIO/OniaIO.h"
#include "../../OniaBase/Params/Params.h"
#include "../Common/Skimmer.h"

template<typename Reader>
class OniaCutterBase : public Cutter<Reader>
{
    private:
    CutParams kineCut;

    public:
    OniaCutterBase(const CutParams* cut): kineCut(*cut) {};

    bool cut(const Reader* input, Int_t index,Int_t entry) override {return kineCut.cut(input,index,entry);}
};

#if defined(__CLING__)
#include "OniaCutter.cpp"
#endif

#endif