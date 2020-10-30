
#include "OniaCutter.h"

OniaCutter::OniaCutter(const cutParams* cut):
kineCut(*cut)
{
}

bool OniaCutter::operator()(Onia_Input* input,Int_t index)
{
    //check for triggers
    if ((input->trig[index] & kineCut.trigSelect) != kineCut.trigSelect) return false;

    if ((kineCut.checkSign) && (input->sign[index] != kineCut.sign)) return false;

    int mupl_idx = input->mupl_idx[index];//plus muon index
    int mumi_idx = input->mumi_idx[index];//minus muon index

    //check if missing index, missing when equals -1
    if ((mupl_idx <0) || (mumi_idx<0)) return false;

    if(kineCut.isMC)
    {
        if(input->whichGen[mupl_idx] == -1) return false;
        if(input->whichGen[mumi_idx] == -1) return false;
    }

    //check if plus and minus muons are soft
    if (!isSoft(input,mupl_idx)) return false;
    if (!isSoft(input,mumi_idx)) return false;

    //kinetic cut
    TLorentzVector* mom4= (TLorentzVector*) input->mom4_QQ->At(index);
    TLorentzVector* mom4_mumi = (TLorentzVector*) input->mom4_mu->At(input->mumi_idx[index]);
    TLorentzVector* mom4_mupl = (TLorentzVector*) input->mom4_mu->At(input->mupl_idx[index]);

    if ((mom4->Pt() < kineCut.ptLow) || (mom4->Pt() > kineCut.ptHigh)) return false;
    if (( fabs(mom4->Rapidity()) < kineCut.yLow) || (fabs(mom4->Rapidity()) >kineCut.yHigh)) return false;
    
    if (mom4_mumi->Pt() < kineCut.singleMuPtLow) return false;
    if (fabs(mom4_mumi->Eta()) > kineCut.singleMuEtaHigh) return false;
    if (mom4_mupl->Pt() < kineCut.singleMuPtLow) return false;
    if (fabs(mom4_mupl->Eta()) > kineCut.singleMuEtaHigh) return false;

    if (input->VtxProb[index] < kineCut.minVtxProb) return false;

    return true;
}

bool OniaCutter::isSoft(Onia_Input* input,Int_t index)
{
    bool passMuonTypePl = (input->SelectionType[index] & kineCut.selectionBits) == (kineCut.selectionBits);
    bool muplSoft = passMuonTypePl && ( input->nTrkWMea[index] >= kineCut.minTracks)
    && ( input->nPixWMea[index] >= kineCut.minPixels)
    && ( fabs(input->dxy[index]) < kineCut.maxDxy)
    && ( fabs(input->dz[index]) < kineCut.maxDz);
    return muplSoft;
}