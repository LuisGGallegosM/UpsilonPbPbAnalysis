
#ifndef ONIADATA
#define ONIADATA

#include "TClonesArray.h"
#include "../TreeProcessor/TreeReader.h"

#define maxBranchSize (32)

struct OniaGenQQ
{
    std::unique_ptr<TClonesArray> genQQ_mom4;
    Int_t genQQ_size;
    Int_t genQQ_mupl_idx[maxBranchSize];
    Int_t genQQ_mumi_idx[maxBranchSize];
    Int_t genQQ_id[maxBranchSize];

    OniaGenQQ(TreeReader* reader);
};

struct OniaGenQQ2
{
    std::unique_ptr<TClonesArray> genQQ_mom4;
    Int_t genQQ_size;
    std::unique_ptr<TClonesArray> genQQ_mupl_mom4;
    std::unique_ptr<TClonesArray> genQQ_mumi_mom4;
    Int_t genQQ_id[maxBranchSize];

    OniaGenQQ2(TreeReader* reader);
};

struct OniaRecoQQ
{
    std::unique_ptr<TClonesArray> recoQQ_mom4;
    Int_t recoQQ_size;
    Int_t recoQQ_mupl_idx[maxBranchSize];
    Int_t recoQQ_mumi_idx[maxBranchSize];
    Int_t recoQQ_sign[maxBranchSize];
    Float_t recoQQ_VtxProb[maxBranchSize];
    ULong64_t recoQQ_trig[maxBranchSize];

    OniaRecoQQ(TreeReader* reader);
};

struct OniaGenMu
{
    std::unique_ptr<TClonesArray> genMu_mom4;
    Int_t genMu_size;

    OniaGenMu(TreeReader* reader);
};

struct OniaRecoMu
{
    std::unique_ptr<TClonesArray> recoMu_mom4;
    Int_t recoMu_size;
    Int_t recoMu_SelectionType[maxBranchSize];
    Int_t recoMu_nTrkWMea[maxBranchSize];
    Int_t recoMu_nPixWMea[maxBranchSize];
    Float_t recoMu_dxy[maxBranchSize];
    Float_t recoMu_dz[maxBranchSize];
    OniaRecoMu(TreeReader* reader);
};

struct OniaWhich
{
    Int_t RecoMuWhichGen[maxBranchSize];
    Int_t GenQQWhichReco[maxBranchSize];
    Int_t GenMuWhichReco[maxBranchSize];
    Int_t RecoQQWhichGen[maxBranchSize];

    OniaWhich(TreeReader* reader);
};

class OniaReader : public TreeReader, 
    public OniaGenQQ, public OniaRecoQQ, public OniaGenMu, public OniaRecoMu, public OniaWhich
{
    public:
    OniaReader(TTree* treeIn);
};

class OniaReader2 : public TreeReader, public OniaGenQQ2, public OniaGenMu
{
    public:
    OniaReader2(TTree* treeIn);
};

#endif