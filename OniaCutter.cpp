
#include "OniaCutter.h"

OniaCutter::OniaCutter()
{
    selectionBits = ((1 <<1) | (1 << 3));
    minTracks =6;
    minPixels =1;
    maxDxy = 0.3f;
    maxDz = 20.0f;
    minVtxProb = 0.01f;
    isMC = true;
    trigSelect = HLT_HIL1DoubleMuOpen_v1;
}

bool OniaCutter::operator()(Onia_Input* input,Long64_t index)
{
    //check for triggers
    if ((input->trig[index] & trigSelect) == trigSelect) return false;

    int mupl_idx = input->mupl_idx[index];//plus muon index
    int mumi_idx = input->mumi_idx[index];//minus muon index

    //cehck if missing index, missing when equals -1
    if ((mupl_idx <0) || (mumi_idx<0)) return false;

    //check if plus and minus muons are soft
    if (!isSoft(input,mupl_idx)) return false;
    if (!isSoft(input,mumi_idx)) return false;

    //check for VtxProb
    if (input->VtxProb[index] < minVtxProb) return false;

    return true;
}

bool OniaCutter::isSoft(Onia_Input* input,Long64_t index)
{
    bool passMuonTypePl = (input->SelectionType[index] & selectionBits) == (selectionBits);
    bool muplSoft = passMuonTypePl && ( input->nTrkWMea[index] >= minTracks)
    && ( input->nPixWMea[index] >= minPixels)
    && ( fabs(input->dxy[index]) < maxDxy)
    && ( fabs(input->dz[index]) < maxDz);
    return muplSoft;
}