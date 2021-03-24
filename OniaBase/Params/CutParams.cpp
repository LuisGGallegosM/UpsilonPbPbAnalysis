
#include <algorithm>

#include "../../Utils/Serialization/Serialization.h"
#include "CutParams.h"

void CutParams::deserialize(const std::string& filename)
{
    Serializer ser(filename);
    ser.read("isMC",isMC);
    ser.read("trigSelect", trigSelect);
    ser.read("checkSign",checkSign );
    ser.read("sign",sign);
    ser.read("prescale",prescale);

    ser.read("selectionBits",selectionBits);
    ser.read("minTracks",minTracks);
    ser.read("minPixels",minPixels);

    ser.read("maxDxy",maxDxy);
    ser.read("maxDz",maxDz);
    ser.read("minVtxProb",minVtxProb);

    ser.read("ptLow",ptLow);
    ser.read("ptHigh",ptHigh);
    ser.read("yLow",yLow);
    ser.read("yHigh",yHigh);
    ser.read("singleMuPtLow",singleMuPtLow);
    ser.read("singleMuEtaHigh",singleMuEtaHigh);
}

bool CutParams::isMatchedQQ(const OniaWhich* which,int mupl_idx, int mumi_idx, int index) const
{
    //gen index of the two muon in generated muon array
    Int_t genMuonPl = which->RecoMuWhichGen[mupl_idx];
    Int_t genMuonMi = which->RecoMuWhichGen[mumi_idx];
    Int_t genQQidx = which->RecoQQWhichGen[index];
    //check if both muons are matched to generated muons
    if(genMuonPl < 0) return false;
    if(genMuonMi < 0) return false;
    if(genQQidx <0) return false;
    
    return true;
}

bool CutParams::kineticQQCut(const OniaRecoQQ* recoQQ, int index)
{
    TLorentzVector* mom4= (TLorentzVector*) recoQQ->mom4->At(index);
    if (mom4==nullptr) return false;
    if ((mom4->Pt() < ptLow) || (mom4->Pt() > ptHigh)) return false;
    if (( fabs(mom4->Rapidity()) < yLow) || (fabs(mom4->Rapidity()) >yHigh)) return false;
    if (recoQQ->VtxProb[index] < minVtxProb) return false;
    return true;
}

bool CutParams::kineticMuCut(const OniaRecoMu* recoMu, int mupl_idx, int mumi_idx)
{
    TLorentzVector* mom4_mupl = (TLorentzVector*) recoMu->mom4->At(mupl_idx);
    TLorentzVector* mom4_mumi = (TLorentzVector*) recoMu->mom4->At(mumi_idx);
    if((mom4_mumi==nullptr) || (mom4_mupl==nullptr)) return false;
    if (mom4_mumi->Pt() < singleMuPtLow) return false;
    if (fabs(mom4_mumi->Eta()) > singleMuEtaHigh) return false;
    if (mom4_mupl->Pt() < singleMuPtLow) return false;
    if (fabs(mom4_mupl->Eta()) > singleMuEtaHigh) return false;
    return true;
}

bool CutParams::isTriggered(const OniaRecoQQ* recoQQ, int index)
{
     //check for triggers
    if ((recoQQ->trig[index] & trigSelect) != trigSelect) return false;
    if ((checkSign) && (recoQQ->sign[index] != sign)) return false;
    return true;
}

template<>
bool CutParams::cut<OniaMCData>(const OniaMCData* input,Int_t index,Int_t entry)
{
    if (!isTriggered(&input->recoQQ,index)) return false;
    int mupl_idx = input->recoQQ.mupl_idx[index];//plus muon index
    int mumi_idx = input->recoQQ.mumi_idx[index];//minus muon index
    if ((mupl_idx <0) || (mumi_idx<0)) return false;

    //check if muon is matched
    if(!isMatchedQQ(&input->which,mupl_idx,mumi_idx,index)) return false;
    //check if plus and minus muons are soft
    if (!isSoft(&input->recoMu,mupl_idx)) return false;
    if (!isSoft(&input->recoMu,mumi_idx)) return false;
    //kinetic cut
    if (!kineticQQCut(&input->recoQQ,index)) return false;
    if (!kineticMuCut(&input->recoMu,mupl_idx,mumi_idx)) return false;
    
    return true;
}

template<>
bool CutParams::cut<OniaJetMCData>(const OniaJetMCData* input,Int_t index,Int_t entry)
{
    if (!isTriggered(&input->recoQQ,index)) return false;
    int mupl_idx = input->recoQQ.mupl_idx[index];//plus muon index
    int mumi_idx = input->recoQQ.mumi_idx[index];//minus muon index
    if ((mupl_idx <0) || (mumi_idx<0)) return false;

    //check if muon is matched
    if(!isMatchedQQ(&input->which,mupl_idx,mumi_idx,index)) return false;
    //check if plus and minus muons are soft
    if (!isSoft(&input->recoMu,mupl_idx)) return false;
    if (!isSoft(&input->recoMu,mumi_idx)) return false;
    //kinetic cut
    if (!kineticQQCut(&input->recoQQ,index)) return false;
    if (!kineticMuCut(&input->recoMu,mupl_idx,mumi_idx)) return false;
    
    return true;
}

bool CutParams::isSoft(const OniaRecoMu* recoMu,Int_t index) const
{
    bool passMuonTypePl = (recoMu->SelectionType[index] & selectionBits) == (selectionBits);
    bool muplSoft = passMuonTypePl && (recoMu->nTrkWMea[index] >= minTracks)
    && ( recoMu->nPixWMea[index] >= minPixels)
    && ( fabs(recoMu->dxy[index]) < maxDxy)
    && ( fabs(recoMu->dz[index]) < maxDz);
    return muplSoft;
}