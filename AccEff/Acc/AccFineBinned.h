
#ifndef ACCFINEBINNED
#define ACCFINEBINNED

#include"TTree.h"
#include"TClonesArray.h"

class AcceptanceFinebinned
{
    TClonesArray    *Gen_QQ_4mom;
    TClonesArray    *Gen_mu_4mom;
    TClonesArray    *Gen_QQ_mupl_4mom;
    TClonesArray    *Gen_QQ_mumi_4mom;
    Int_t            Gen_QQ_size;
    Int_t            Gen_QQ_mupl_idx[1000];
    Int_t            Gen_QQ_mumi_idx[1000];
    Int_t            Gen_QQ_whichRec[1000];
    Int_t            Gen_mu_whichRec[1000];
    public:

    AcceptanceFinebinned()
    {
        memset( this, 0, sizeof(AcceptanceFinebinned) );
    }

    void calculate(const char* filename, const char* outfilename);
};

void AccFineBinning(const char* filename, const char* outfilename);

#endif