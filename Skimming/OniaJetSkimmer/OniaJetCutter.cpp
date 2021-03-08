

#include"OniaJetCutter.h"

template<typename Reader>
bool OniaJetCutterBase<Reader>::cut(const Reader* input, Int_t index,Int_t entry)
{
    return true;
}

template class OniaJetCutterBase<OniaJetReaderMC>;
template class OniaJetCutterBase<OniaJetReaderRealData>;