
#include "JetCutter.h"

JetCutter::JetCutter(Onia_Aux* auxDat) : auxData(auxDat)
{
    minPt = 10.0f;
    maxPt = 50.0f;
}

bool JetCutter::operator()(Jet_Input* input,Int_t index,Int_t entry)
{
    Long64_t evtN=input->evt;

    if (auxData->events.count(evtN)==0) return false;

    if ((input->jtPt[index] < minPt) || (input->jtPt[index] > maxPt)) return false;

    return true;
}