
#ifndef ONIADATA
#define ONIADATA

#include "TClonesArray.h"
#include "../TreeProcessor/TreeReader.h"

#define maxBranchSize (32)

class OniaReader : public TreeReader 
{
    public:
    std::unique_ptr<TClonesArray> genQQ_mom4;
    Int_t genQQ_size;
    Int_t genQQ_mupl_idx[maxBranchSize];
    Int_t genQQ_mumi_idx[maxBranchSize];
    Int_t genQQ_sign[maxBranchSize];

    std::unique_ptr<TClonesArray> recoQQ_mom4;
    Int_t recoQQ_size;
    Int_t recoQQ_mupl_idx[maxBranchSize];
    Int_t recoQQ_mumi_idx[maxBranchSize];
    Int_t recoQQ_sign[maxBranchSize];

    std::unique_ptr<TClonesArray> genMu_mom4;
    Int_t genMu_size;

    std::unique_ptr<TClonesArray> recoMu_mom4;
    Int_t recoMu_size;

    Int_t SelectionType[maxBranchSize];
    Int_t nTrkWMea[maxBranchSize];
    Int_t nPixWMea[maxBranchSize];
    Float_t dxy[maxBranchSize];
    Float_t dz[maxBranchSize];
    Float_t VtxProb[maxBranchSize];
    ULong64_t trig[maxBranchSize];
    Int_t RecoMuWhichGen[maxBranchSize];
    Int_t GenQQWhichReco[maxBranchSize];
    Int_t GenMuWhichReco[maxBranchSize];
    Int_t RecoQQWhichGen[maxBranchSize];

    Int_t genQQid[maxBranchSize];

    std::unique_ptr<TClonesArray> genQQ_mupl_mom4;
    std::unique_ptr<TClonesArray> genQQ_mumi_mom4;

    OniaReader(TTree* treeIn);
};

#endif