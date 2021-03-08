
#include"OniaJetSkimmer.h"
#include "TLorentzVector.h"

std::unique_ptr<Skimmer> createJetSkimmer(TTree* tree ,const CutParams* cutter, const char* outTreeName)
{
    if (cutter->getIsMC())
    {
        OniaJetCutterMC* oniaCutter =new OniaJetCutterMC(cutter);
        return std::unique_ptr<Skimmer>(new OniaJetSkimmerMC(tree,oniaCutter,outTreeName));
    }
    else
    {
        OniaJetCutterRealData* oniaCutter = new OniaJetCutterRealData(cutter);
        return std::unique_ptr<Skimmer>(new OniaJetSkimmerRealData(tree,oniaCutter,outTreeName));
    } 
}
