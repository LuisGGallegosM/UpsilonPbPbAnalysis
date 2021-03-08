

#include "OniaSkimmer.h"
#include "TLorentzVector.h"

std::unique_ptr<Skimmer> createOniaSkimmer(TTree* tree ,const CutParams* cutter, const char* outTreeName)
{
    if (cutter->getIsMC())
    {
        OniaCutterBase<OniaReaderMC>* oniaCutter =new OniaCutterBase<OniaReaderMC>(cutter);
        return std::unique_ptr<Skimmer>(new OniaSkimmerMC(tree,oniaCutter,outTreeName));
    }
    else
    {
        OniaCutterBase<OniaReaderRealData>* oniaCutter = new OniaCutterBase<OniaReaderRealData>(cutter);
        return std::unique_ptr<Skimmer>(new OniaSkimmerReadData(tree,oniaCutter,outTreeName));
    } 
}

