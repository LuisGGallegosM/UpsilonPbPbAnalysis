#include "AccEffCutter.h"
#include "TLorentzVector.h"

AccEffCutter::AccEffCutter(const cutParams* cut):
kineCut(*cut)
{
}

bool AccEffCutter::cut(AccEffInput* input,Int_t index,Int_t entry)
{
    return true;
}