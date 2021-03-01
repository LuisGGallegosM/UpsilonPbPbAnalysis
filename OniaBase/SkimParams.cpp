
#include "SkimParams.h"
#include "TLorentzVector.h"

void cutParams::deserialize(const std::string& filename)
{
    serializer ser(filename);
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

bool cutParams::cut(const OniaReader* input,Int_t index,Int_t entry)
{
    //check for triggers
    if ((input->trig[index] & trigSelect) != trigSelect) return false;

    if ((checkSign) && (input->recoQQ_sign[index] != sign)) return false;

    int mupl_idx = input->recoQQ_mupl_idx[index];//plus muon index
    int mumi_idx = input->recoQQ_mumi_idx[index];//minus muon index

    //check if missing index, missing when equals -1
    if ((mupl_idx <0) || (mumi_idx<0)) return false;

    if(isMC)
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
    TLorentzVector* mom4= (TLorentzVector*) input->recoQQ_mom4->At(index);
    TLorentzVector* mom4_mumi = (TLorentzVector*) input->recoMu_mom4->At(input->recoQQ_mumi_idx[index]);
    TLorentzVector* mom4_mupl = (TLorentzVector*) input->recoMu_mom4->At(input->recoQQ_mupl_idx[index]);

    if((mom4==nullptr) || (mom4_mumi==nullptr) || (mom4_mupl==nullptr))
    {
        std::cerr << "TLorentzVector reading error at entry:" << entry <<", muon index:"<< index << '\n';
        return false;
    }

    if ((mom4->Pt() < ptLow) || (mom4->Pt() > ptHigh)) return false;
    if (( fabs(mom4->Rapidity()) < yLow) || (fabs(mom4->Rapidity()) >yHigh)) return false;
    
    if (mom4_mumi->Pt() < singleMuPtLow) return false;
    if (fabs(mom4_mumi->Eta()) > singleMuEtaHigh) return false;
    if (mom4_mupl->Pt() < singleMuPtLow) return false;
    if (fabs(mom4_mupl->Eta()) > singleMuEtaHigh) return false;

    if (input->VtxProb[index] < minVtxProb) return false;

    return true;
}

bool cutParams::isSoft(const OniaReader* input,Int_t index) const
{
    bool passMuonTypePl = (input->SelectionType[index] & selectionBits) == (selectionBits);
    bool muplSoft = passMuonTypePl && ( input->nTrkWMea[index] >= minTracks)
    && ( input->nPixWMea[index] >= minPixels)
    && ( fabs(input->dxy[index]) < maxDxy)
    && ( fabs(input->dz[index]) < maxDz);
    return muplSoft;
}