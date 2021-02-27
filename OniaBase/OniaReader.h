
#ifndef ONIADATA
#define ONIADATA

#include "TClonesArray.h"
#include <unordered_map>
#include "../TreeProcessor/TreeReader.h"

#define maxBranchSize (64)

struct baseMu
{
    TClonesArray* mom4;
    Int_t size;
    baseMu();
    ~baseMu();      
};
struct baseQQ
{
    TClonesArray* mom4;
    Int_t size;
    Int_t* mupl_idx;
    Int_t* mumi_idx;
    Int_t* sign;
    Int_t* id;
    baseQQ();
    ~baseQQ();
};

class OniaReader : public TreeReader 
{
    public:
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

    OniaReader(TTree* treeIn, bool isMC);
    ~OniaReader();
};

#endif