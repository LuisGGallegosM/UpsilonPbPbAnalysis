#ifndef ONIAJETCUTTER
#define ONIAJETCUTTER

#include "../../OniaBase/OniaReader.h"
#include "../../OniaBase/CutParams.h"
#include "../Skimmer.h"

template<typename Reader>
class OniaJetCutterBase : public Cutter<Reader>
{
    private:
    CutParams kineCut;

    public:
    OniaJetCutterBase(const CutParams* cut): kineCut(*cut) {};

    bool cut(const Reader* input, Int_t index,Int_t entry) override;
};

using OniaJetCutterMC = OniaJetCutterBase<OniaJetReaderMC>;
using OniaJetCutterRealData = OniaJetCutterBase<OniaJetReaderRealData>;

#endif