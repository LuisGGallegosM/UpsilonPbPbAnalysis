#ifndef SKIMPARAMS
#define SKIMPARAMS

#include "../OniaIO/OniaIO.h"
#include "TLorentzVector.h"

/**
 * @brief stores the information for cut in skimming, such as
 * pt and y ranges and if is MC or Real Data.
 */
struct CutParams
{
    bool isMC;
    bool checkSign;
    unsigned long long trigSelect;
    int sign;
    int prescale;

    int selectionBits;
    int minTracks;
    int minPixels;
    float maxDxy;
    float maxDz;
    float minVtxProb;

    float ptLow;
    float ptHigh;
    float yLow;
    float yHigh;
    float singleMuPtLow;
    float singleMuEtaHigh;

    CutParams() : isMC(false), checkSign(false), trigSelect(0LL), prescale(0),
    sign(0), selectionBits(0), minPixels(-1),minTracks(-1), maxDxy(-1.0f),maxDz(-1.0f),
    minVtxProb(-1.0f),ptHigh(-1.0f),ptLow(-1.0f),yLow(-1.0f),yHigh(-1.0f),
    singleMuPtLow(-1.0f), singleMuEtaHigh(-1.0f)
    {
    }

    bool isValid() const
    {
        return (minTracks>=0) && (minPixels>=0) && (ptLow>=0.0f) 
        && (ptHigh>0.0f) && (singleMuPtLow>=0.0f) && (maxDxy>0.0f)
        && (maxDz > 0.0f) && (minVtxProb >0.0f) && (prescale>0);
    }


    bool isSoft(const OniaRecoMu* input,Int_t index) const;
    bool isMatchedQQ(const OniaWhich* which,int mupl_idx, int mumi_idx, int index) const;
    bool isPrescaled(Int_t entry) const { return (prescale>1) && ((entry % prescale)!=0); }
    bool isTriggered(const OniaRecoQQ* recoQQ, int index);
    bool kineticQQCut(const OniaRecoQQ* recoQQ,int index);
    bool kineticMuCut(const OniaRecoMu* recoMu, int mupl_idx, int mumi_idx);

    bool getIsMC() const { return isMC;}
    float getMuPtLow() const {return singleMuPtLow;}
    float getMuEtaHigh() const {return singleMuEtaHigh;}

    void deserialize(const std::string& filename);

    template<typename Reader>
    bool cut(const Reader* input,Int_t index,Int_t entry)
    {
        if (!isTriggered(&input->recoQQ,index)) return false;

        int mupl_idx = input->recoQQ.mupl_idx[index];//plus muon index
        int mumi_idx = input->recoQQ.mumi_idx[index];//minus muon index
        if ((mupl_idx <0) || (mumi_idx<0)) return false;

        //check if plus and minus muons are soft
        if (!isSoft(&input->recoMu,mupl_idx)) return false;
        if (!isSoft(&input->recoMu,mumi_idx)) return false;
        //kinetic cut
        if (!kineticQQCut(&input->recoQQ,index)) return false;
        if (!kineticMuCut(&input->recoMu,mupl_idx,mumi_idx)) return false;
        return true;
    }
};

template<>
bool CutParams::cut<OniaMCData>(const OniaMCData* input,Int_t index,Int_t entry);

template<>
bool CutParams::cut<OniaJetMCData>(const OniaJetMCData* input,Int_t index,Int_t entry);

#endif