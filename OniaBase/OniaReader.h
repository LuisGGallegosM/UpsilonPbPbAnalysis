
#ifndef ONIADATA
#define ONIADATA

#include "TClonesArray.h"
#include "../TreeProcessor/TreeReader.h"

#define maxBranchSize (32)

struct OniaGenQQ
{
    std::unique_ptr<TClonesArray> mom4;
    Int_t size;
    Int_t mupl_idx[maxBranchSize];
    Int_t mumi_idx[maxBranchSize];
    Int_t id[maxBranchSize];

    void addInputs(TreeReader* reader);
};

struct OniaGenQQ2
{
    std::unique_ptr<TClonesArray> mom4;
    Int_t size;
    std::unique_ptr<TClonesArray> mupl_mom4;
    std::unique_ptr<TClonesArray> mumi_mom4;
    Int_t id[maxBranchSize];

    void addInputs(TreeReader* reader);
};

struct OniaRecoQQ
{
    std::unique_ptr<TClonesArray> mom4;
    Int_t size;
    Int_t mupl_idx[maxBranchSize];
    Int_t mumi_idx[maxBranchSize];
    Int_t sign[maxBranchSize];
    Float_t VtxProb[maxBranchSize];
    ULong64_t trig[maxBranchSize];

    void addInputs(TreeReader* reader);
};

struct OniaGenMu
{
    std::unique_ptr<TClonesArray> mom4;
    Int_t size;

    void addInputs(TreeReader* reader);
};

struct OniaRecoMu
{
    std::unique_ptr<TClonesArray> mom4;
    Int_t size;
    Int_t SelectionType[maxBranchSize];
    Int_t nTrkWMea[maxBranchSize];
    Int_t nPixWMea[maxBranchSize];
    Float_t dxy[maxBranchSize];
    Float_t dz[maxBranchSize];
    void addInputs(TreeReader* reader);
};

struct OniaWhich
{
    Int_t RecoMuWhichGen[maxBranchSize];
    Int_t GenQQWhichReco[maxBranchSize];
    Int_t GenMuWhichReco[maxBranchSize];
    Int_t RecoQQWhichGen[maxBranchSize];

    void addInputs(TreeReader* reader);
};

class OniaReader
{
    private:
    TreeReader reader;
    
    public:
    OniaGenQQ genQQ;
    OniaRecoQQ recoQQ;
    OniaGenMu genMu;
    OniaRecoMu recoMu;
    OniaWhich which;

    OniaReader(TTree* treeIn);
    const TreeReader* getReader() const {return &reader;}
};

class OniaReader2
{
    private:
    TreeReader reader;
    public:
    OniaGenQQ2 genQQ;
    OniaGenMu genMu;
    OniaReader2(TTree* treeIn);
    const TreeReader* getReader() const {return &reader;}
};

#endif