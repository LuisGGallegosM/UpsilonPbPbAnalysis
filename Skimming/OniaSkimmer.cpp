

#include "OniaSkimmer.h"
#include "TLorentzVector.h"

OniaSkimmer::OniaSkimmer(TTree* treeIn, OniaOutputer* outp , OniaCutter* cut) 
: TreeProcessor(), cutter(cut), outputer(outp), dataIn(treeIn,cut->isMC())
{
    return;
}

void OniaSkimmer::ProcessEvent(Long64_t entry)
{
    if (cutter->prescale(entry)) return;
    Long64_t size=dataIn.getSizeRecoQQ();
    
    for(Long64_t i=0;i<size;++i)
    {
        if (cutter->cut(&dataIn,i,entry))
        {
            outputer->WriteData(dataIn,i,entry);
        }
    }
}