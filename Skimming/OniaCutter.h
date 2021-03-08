#ifndef ONIACUTTER
#define ONIACUTTER

#include "../OniaBase/OniaReader.h"
#include "../OniaBase/CutParams.h"

template<typename Reader>
class OniaCutter
{
    public:
    virtual bool cut(const Reader* input, Int_t index,Int_t entry) = 0;
    virtual ~OniaCutter() = default;
};

template<typename Reader>
class OniaCutterBase : public OniaCutter<Reader>
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