#ifndef ONIADATAQQ
#define ONIADATAQQ

#define maxBranchSize (64)

#include "TClonesArray.h"
#include "../../TreeProcessor/TreeReader.h"

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

struct OniaJetInfo
{
    Int_t pPAprimaryVertexFilter;
    Int_t pBeamScrapingFilter;
    Int_t nref;
    Float_t rawpt[maxBranchSize];
    Float_t pt[maxBranchSize];
    Float_t eta[maxBranchSize];
    Float_t y[maxBranchSize];
    Float_t phi[maxBranchSize];
    Float_t area[maxBranchSize];
    Float_t m[maxBranchSize];
    
    Float_t refpt[maxBranchSize];
    Float_t refeta[maxBranchSize];
    Float_t refy[maxBranchSize];
    Float_t refphi[maxBranchSize];
    Float_t refm[maxBranchSize];
    Float_t refarea[maxBranchSize];

    void addInputs(TreeReader* reader);
};

#endif