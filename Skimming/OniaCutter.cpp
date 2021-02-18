

#include "OniaCutter.h"
#include "TLorentzVector.h"

OniaCutterRecoQQ::OniaCutterRecoQQ(const cutParams* cut):
kineCut(*cut)
{
}

bool OniaCutterRecoQQ::cut(const Onia_Input* input,Int_t index,Int_t entry)
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
        //gen index of the two muon in generated muon array
        Int_t genMuonPl = input->RecoMuWhichGen[mupl_idx];
        Int_t genMuonMi = input->RecoMuWhichGen[mumi_idx];
        //check if both muons are matched to generated muons
        if(genMuonPl < 0) return false;
        if(genMuonMi < 0) return false;
    }

    //check if plus and minus muons are soft
    if (!isSoft(input,mupl_idx)) return false;
    if (!isSoft(input,mumi_idx)) return false;

    //kinetic cut
    TLorentzVector* mom4= (TLorentzVector*) input->mom4_RecoQQ->At(index);
    TLorentzVector* mom4_mumi = (TLorentzVector*) input->mom4_RecoMu->At(input->mumi_idx[index]);
    TLorentzVector* mom4_mupl = (TLorentzVector*) input->mom4_RecoMu->At(input->mupl_idx[index]);

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

bool OniaCutterRecoQQ::isSoft(const Onia_Input* input,Int_t index) const
{
    bool passMuonTypePl = (input->SelectionType[index] & kineCut.selectionBits) == (kineCut.selectionBits);
    bool muplSoft = passMuonTypePl && ( input->nTrkWMea[index] >= kineCut.minTracks)
    && ( input->nPixWMea[index] >= kineCut.minPixels)
    && ( fabs(input->dxy[index]) < kineCut.maxDxy)
    && ( fabs(input->dz[index]) < kineCut.maxDz);
    return muplSoft;
}