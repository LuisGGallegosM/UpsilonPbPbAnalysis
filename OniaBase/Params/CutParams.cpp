
#include <algorithm>

#include "../../Utils/Serialization/Serialization.h"
#include "CutParams.h"

void CutParams::deserialize(const std::string& filename)
{
    ParameterGroup g;
    g.deserialize(filename);

    isMC=g.getBool("isMC");
    trigSelect=g.getInt("trigSelect");

    checkSign = g.getBool("checkSign");
    sign = g.getInt("sign");
    prescale =g.getInt("prescale");

    selectionBits = g.getInt("selectionBits");
    minTracks = g.getInt("minTracks");
    minPixels = g.getInt("minPixels");

    maxDxy = g.getFloat("maxDxy");
    maxDz = g.getFloat("maxDz");
    minVtxProb = g.getFloat("minVtxProb");

    ptLow = g.getFloat("ptLow");
    ptHigh = g.getFloat("ptHigh");
    yLow = g.getFloat("yLow");
    yHigh = g.getFloat("yHigh");
    singleMuPtLow = g.getFloat("singleMuPtLow");
    singleMuEtaHigh = g.getFloat("singleMuEtaHigh");
}

bool isMatchedQQ(const OniaWhich* which,int mupl_idx, int mumi_idx, int index)
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

bool CutParams::isMatchedQQ(const OniaWhich* which,int mupl_idx, int mumi_idx, int index) const
{
    return ::isMatchedQQ(which,mupl_idx,mumi_idx,index);
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

int FindJetGen(const OniaJetMCData* input,int iQQ)
{
    auto jetInfo = &(input->jetRef);
    const TLorentzVector* GenQQ4mom= (TLorentzVector*) input->genQQ.mom4->At(iQQ);
    const int jetNumber= input->jetInfo.nref;
    const float drmin=0.5f;
    int result=-1;
    for(int iJet=0;iJet< jetNumber;iJet++)
    {
        float jt_pt=input->jetRef.refpt[iJet];
        if (jt_pt < 0.0f) continue;
        float jt_eta=jetInfo->refeta[iJet];
        float jt_phi=jetInfo->refphi[iJet];
        float z = GenQQ4mom->Pt()/jt_pt;

        if ((z<0.0f) || (z>1.0f)) continue;
        if (jt_eta > 2.0) continue;

        TLorentzVector v_jet;
        v_jet.SetPtEtaPhiM(jt_pt, jt_eta, jt_phi, jetInfo->refm[iJet]);
        if (GenQQ4mom->DeltaR(v_jet)<=drmin)
        {
            result=iJet;
            break;
        }
    }
    return result;
}

int FindJetNoCorr(const OniaJetMCData* input, int iQQ)
{
    auto jetInfo = &(input->jetInfo);
    const TLorentzVector* RecoQQ4mom= (TLorentzVector*) input->recoQQ.mom4->At(iQQ);
    const int jetNumber= jetInfo->nref;
    const float drmin=0.5f;
    int result=-1;
    for(int iJet=0;iJet< jetNumber;iJet++)
    {
        if (input->jetRef.refpt[iJet] < 0.0f) continue;
        float jt_eta=jetInfo->eta[iJet];
        float jt_phi=jetInfo->phi[iJet];
        float jt_pt = jetInfo->pt[iJet];
        float z = RecoQQ4mom->Pt()/jt_pt;

        if ((z<0.0f) || (z>1.0f)) continue;
        if (jt_eta > 2.0) continue;

        TLorentzVector v_jet;
        v_jet.SetPtEtaPhiM(jt_pt, jt_eta, jt_phi, jetInfo->m[iJet]);
        if (RecoQQ4mom->DeltaR(v_jet)<=drmin)
        {
            result=iJet;
            break;
        }
    }
    return result;
}

int FindJet(const OniaJetMCData* input, JetCorrector* JEC, int iQQ)
{
    auto jetInfo = &(input->jetInfo);
    const TLorentzVector* RecoQQ4mom= (TLorentzVector*) input->recoQQ.mom4->At(iQQ);
    const int jetNumber= jetInfo->nref;
    const float drmin=0.5f;
    int result=-1;
    for(int iJet=0;iJet< jetNumber;iJet++)
    {
        if (input->jetRef.refpt[iJet] < 0.0f) continue;
        float recoQQpt=RecoQQ4mom->Pt();
        float jt_eta=jetInfo->eta[iJet];
        float jt_phi=jetInfo->phi[iJet];
        float jt_pt_noZJEC = getCorrectedPt(JEC,jetInfo,iJet);
        float jt_pt=jecCorr(jt_pt_noZJEC, input->jetInfo.rawpt[iJet],recoQQpt);
        float z = zTolerance(recoQQpt/jt_pt);

        if ((z<0.0f) || (z>1.0f)) continue;
        if (jt_eta > 2.0) continue;

        TLorentzVector v_jet;
        v_jet.SetPtEtaPhiM(jt_pt_noZJEC, jt_eta, jt_phi, jetInfo->m[iJet]);
        if (RecoQQ4mom->DeltaR(v_jet)<=drmin)
        {
            result=iJet;
            break;
        }
    }
    return result;
}

int FindJet(const OniaJetRealData* input, JetCorrector* JEC, int iQQ)
{
    auto jetInfo = &(input->jetInfo);
    const TLorentzVector* RecoQQ4mom= (TLorentzVector*) input->recoQQ.mom4->At(iQQ);
    const int jetNumber= jetInfo->nref;
    const float drmin=0.5f;
    int result=-1;
    for(int iJet=0;iJet< jetNumber;iJet++)
    {
        float jt_eta=jetInfo->eta[iJet];
        float jt_phi=jetInfo->phi[iJet];
        float jt_pt_noZJEC = getCorrectedPt(JEC,jetInfo,iJet);
        float z = RecoQQ4mom->Pt()/jt_pt_noZJEC;

        if ((z<0.0f) || (z>1.0f)) continue;

        TLorentzVector v_jet;
        v_jet.SetPtEtaPhiM(jt_pt_noZJEC, jt_eta, jt_phi, jetInfo->m[iJet]);
        if (RecoQQ4mom->DeltaR(v_jet)<=drmin)
        {
            result=iJet;
            break;
        }
    }
    return result;
}