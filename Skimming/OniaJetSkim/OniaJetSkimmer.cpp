
#include"OniaJetSkimmer.h"
#include "TLorentzVector.h"

std::unique_ptr<Skimmer> createJetSkimmer(TTree* tree ,const CutParams* cutter, const char* outTreeName)
{
    if (cutter->getIsMC())
    {
        return std::unique_ptr<Skimmer>(new OniaJetSkimmerMC(tree,cutter,outTreeName));
    }
    else
    {
        return std::unique_ptr<Skimmer>(new OniaJetSkimmerRealData(tree,cutter,outTreeName));
    }
}