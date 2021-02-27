
#ifndef ONIADATA
#define ONIADATA

#include "TClonesArray.h"
#include <unordered_map>
#include "../TreeProcessor/TreeReader.h"

#define maxBranchSize (64)

class OniaInput : public TreeReaderBase 
{
    public:
    struct baseQQ
    {
        TClonesArray* mom4;
        Int_t size;
        Int_t* mupl_idx;
        Int_t* mumi_idx;
        Int_t* sign;
        baseQQ();
        ~baseQQ();
    };
    struct baseMu
    {
        TClonesArray* mom4;
        Int_t size;
        baseMu();
        ~baseMu();      
    };
    baseQQ recoQQ;
    baseQQ genQQ;
    baseMu recoMu;
    baseMu genMu;

    Int_t* SelectionType;
    Int_t* nTrkWMea;
    Int_t* nPixWMea;
    Float_t* dxy;
    Float_t* dz;
    Float_t* VtxProb;
    ULong64_t* trig;
    UInt_t runNb;
    UInt_t eventNb;
    UInt_t lumi;
    Int_t* RecoMuWhichGen;
    Int_t* GenMuWhichReco;
    Int_t* GenQQWhichReco;
    Int_t* GenQQid;
    
    Int_t getSizeRecoQQ() const {return recoQQ.size;}
    Int_t getSizeGenQQ() const {return genQQ.size;}
    Int_t getSizeRecoMu() const {return recoMu.size;}
    Int_t getSizeGenMu() const {return genMu.size;}

    OniaInput(TTree* treeIn, bool isMC);
    ~OniaInput();
};

#endif