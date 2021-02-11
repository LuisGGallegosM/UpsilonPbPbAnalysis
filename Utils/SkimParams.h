#ifndef SKIMPARAMS
#define SKIMPARAMS

#include "serialize.h"

//changed to Jpsi that is currently hardcoded to Jpsi
//Upsilon Y1S PDG ID
#define UPSILON_Y1S_PDGID (443)
//#define UPSILON_Y1S_PDGID (553) true PDG

struct cutParams
{
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

    void deserialize(const std::string& filename);
};

#endif