

#include "OniaSkimmer.h"
#include "TLorentzVector.h"


std::unique_ptr<Skimmer> createOniaSkimmer(TTree* tree ,const CutParams* cutter, const char* outTreeName)
{
    if (cutter->getIsMC())
    {
        OniaCutterBase<OniaMCData>* oniaCutter =new OniaCutterBase<OniaMCData>(cutter);
        return std::unique_ptr<Skimmer>(new OniaSkimmerMC(tree,oniaCutter,outTreeName));
    }
    else
    {
        OniaCutterBase<OniaRealData>* oniaCutter = new OniaCutterBase<OniaRealData>(cutter);
        return std::unique_ptr<Skimmer>(new OniaSkimmerReadData(tree,oniaCutter,outTreeName));
    } 
}

