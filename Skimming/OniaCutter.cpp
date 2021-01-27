
#include "OniaCutter.h"

OniaCutter::OniaCutter(const cutParams* cut):
kineCut(*cut)
{
}

bool OniaCutter::cut(Onia_Input* input,Int_t index,Int_t entry)
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
        if(input->RecoMuWhichGen[mupl_idx] < 0) return false;
        if(input->RecoMuWhichGen[mumi_idx] < 0) return false;

        int genQQindex=input->RecoQQWhichGen[index];
        if (genQQindex < 0) return false;
        int pdgId=input->GenQQid[genQQindex];
        if ((pdgId != UPSILON_Y1S_PDGID ) && ( pdgId != -UPSILON_Y1S_PDGID)) return false;
    }

    //check if plus and minus muons are soft
    if (!isSoft(input,mupl_idx)) return false;
    if (!isSoft(input,mumi_idx)) return false;

    //kinetic cut
    TLorentzVector* mom4= (TLorentzVector*) input->mom4_QQ->At(index);
    TLorentzVector* mom4_mumi = (TLorentzVector*) input->mom4_mu->At(input->mumi_idx[index]);
    TLorentzVector* mom4_mupl = (TLorentzVector*) input->mom4_mu->At(input->mupl_idx[index]);

    if((mom4==nullptr) || (mom4_mumi==nullptr) || (mom4_mupl==nullptr))
    {
        std::cerr << "TLorentzVector reading error at entry:" << entry <<", muon index:"<< index << '\n';
        return false;
    }

    if ((mom4->Pt() < kineCut.ptLow) || (mom4->Pt() > kineCut.ptHigh)) return false;
    if (( fabs(mom4->Rapidity()) < kineCut.yLow) || (fabs(mom4->Rapidity()) >kineCut.yHigh)) return false;
    
    if (mom4_mumi->Pt() < kineCut.singleMuPtLow) return false;
    if (fabs(mom4_mumi->Eta()) > kineCut.singleMuEtaHigh) return false;
    if (mom4_mupl->Pt() < kineCut.singleMuPtLow) return false;
    if (fabs(mom4_mupl->Eta()) > kineCut.singleMuEtaHigh) return false;

    if (input->VtxProb[index] < kineCut.minVtxProb) return false;

    return true;
}

bool OniaCutter::prescale(Int_t entry)
{
    return (kineCut.prescale>1) && ((entry % kineCut.prescale)!=0);
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