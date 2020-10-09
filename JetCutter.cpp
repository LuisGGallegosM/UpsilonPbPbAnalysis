
#include "JetCutter.h"

JetCutter::JetCutter(Onia_Aux* auxDat) : auxData(auxDat)
{

}

bool JetCutter::operator()(Jet_Input* input,Int_t index)
{
    Long64_t evtN=input->evt;
    if (auxData->evNumGot.count(evtN)==0) return false;

    return true;
}