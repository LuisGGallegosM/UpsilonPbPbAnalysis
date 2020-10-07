#include "JetCutter.h"

JetCutter::JetCutter()
{

}

bool JetCutter::operator()(Jet_Input* input,Int_t index)
{
    return true;
}