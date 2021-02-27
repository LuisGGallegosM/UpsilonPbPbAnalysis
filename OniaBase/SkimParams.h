#ifndef SKIMPARAMS
#define SKIMPARAMS

#include "../Utils/serialize.h"
#include "OniaReader.h"

//changed to Jpsi that is currently hardcoded to Jpsi
//Upsilon Y1S PDG ID
#define UPSILON_Y1S_PDGID (553) true PDG

class cutParams
{
    private:
    bool isMC;
    bool checkSign;
    unsigned long long trigSelect;
    int sign;
    int prescale;
    bool genLoop;

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

    public:
    cutParams() : isMC(false), checkSign(false), trigSelect(0LL), prescale(0), genLoop(false),
    sign(0), selectionBits(0), minPixels(-1),minTracks(-1), maxDxy(-1.0f),maxDz(-1.0f),
    minVtxProb(-1.0f),ptHigh(-1.0f),ptLow(-1.0f),yLow(-1.0f),yHigh(-1.0f),
    singleMuPtLow(-1.0f), singleMuEtaHigh(-1.0f)
    {
    }

    bool isValid() const
    {
        return (minTracks>=0) && (minPixels>=0) && (ptLow>=0.0f) 
        && (ptHigh>0.0f) && (singleMuPtLow>=0.0f) && (maxDxy>0.0f)
        && (maxDz > 0.0f) && (minVtxProb >0.0f) && (prescale>0) && (!(genLoop && (!isMC)));
    }

    bool cut(const OniaReader* input,Int_t index,Int_t entry);
    bool isSoft(const OniaReader* input,Int_t index) const;
    bool isPrescaled(Int_t entry) const { return (prescale>1) && ((entry % prescale)!=0); }

    bool getIsMC() const { return isMC;}
    float getMuPtLow() const {return singleMuPtLow;}
    float getMuEtaHigh() const {return singleMuEtaHigh;}

    void deserialize(const std::string& filename);
};

#endif