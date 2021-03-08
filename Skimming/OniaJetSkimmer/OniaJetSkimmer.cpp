
#include"OniaJetSkimmer.h"
#include "TLorentzVector.h"

template<typename Reader>
OniaJetSkimmer<Reader>::OniaJetSkimmer(TTree* tree , Cutter<Reader>* cutter, const char* outTreeName)
: oniaCutter(cutter) ,oniaReader(tree), oniaWriter(outTreeName)
{
}

template<typename Reader>
void OniaJetSkimmer<Reader>::ProcessEvent(Long64_t entry)
{
    Long64_t size=oniaReader.recoQQ.size;
    
    for(Long64_t i=0;i<size;++i)
    {
        if (oniaCutter->cut(&oniaReader,i,entry))
        {
            oniaWriter.writeQQ(&oniaReader,i,entry);
        }
    }
}

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

template class OniaJetSkimmer<OniaJetReaderMC>;
template class OniaJetSkimmer<OniaJetReaderRealData>;

