#ifndef EFFFINEBINNED
#define EFFFINEBINNED

#include"TClonesArray.h"

class EfficiencyFineBinned
{
    TClonesArray    *Gen_QQ_4mom;
    TClonesArray    *Gen_mu_4mom;
    TClonesArray    *Reco_QQ_4mom;
    TClonesArray    *Reco_mu_4mom;
    Int_t            Gen_QQ_size;
    Int_t            Gen_QQ_mupl_idx[1000];
    Int_t            Gen_QQ_mumi_idx[1000];
    Int_t            Gen_QQ_whichRec[1000];
    Int_t            Gen_mu_whichRec[1000];
    Int_t            Reco_QQ_size;
    Int_t            Reco_QQ_sign[1000];
    ULong64_t        Reco_QQ_trig[1000];
    Int_t            Reco_QQ_mupl_idx[1000];
    Int_t            Reco_QQ_mumi_idx[1000];
    Float_t          Reco_QQ_VtxProb[1000];
    Int_t            Reco_mu_size;
    Int_t            Reco_mu_whichGen[1000];
    Int_t            Reco_mu_nTrkWMea[1000];
    Int_t            Reco_mu_nPixWMea[1000];
    Float_t          Reco_mu_dxy[1000];
    Float_t          Reco_mu_dxyErr[1000];  
    Float_t          Reco_mu_dz[1000];  
    Float_t          Reco_mu_dzErr[1000];
    ULong64_t        Reco_mu_trig[1000];
    Int_t            Reco_mu_SelectionType[1000];
    ULong64_t        HLTriggers;
    Int_t            nTrig;
    Int_t            trigPrescale[15];

    Bool_t Notify();
    public:

    EfficiencyFineBinned()
    {
        memset( this, 0, sizeof(EfficiencyFineBinned) );
    }

    void calculate(const char* filename, const char* outfilename);
    
};

void EffFineBinning(const char* filename, const char* outfilename);

#endif