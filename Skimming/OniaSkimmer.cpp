

#include "OniaSkimmer.h"
#include "TLorentzVector.h"

template<typename Reader>
OniaSkimmerBase<Reader>::OniaSkimmerBase(TTree* tree , OniaCutter<Reader>* cutter, const char* outTreeName)
: oniaCutter(cutter) ,oniaReader(tree), oniaWriter(outTreeName)
{
}

template<typename Reader>
void OniaSkimmerBase<Reader>::ProcessEvent(Long64_t entry)
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

std::unique_ptr<OniaSkimmer> createSkimmer(TTree* tree ,const CutParams* cutter, const char* outTreeName)
{
    if (cutter->getIsMC())
    {
        OniaCutterBase<OniaReaderMC>* oniaCutter =new OniaCutterBase<OniaReaderMC>(cutter);
        return std::unique_ptr<OniaSkimmer>(new OniaSkimmerMC(tree,oniaCutter,outTreeName));
    }
    else
    {
        OniaCutterBase<OniaReaderRealData>* oniaCutter = new OniaCutterBase<OniaReaderRealData>(cutter);
        return std::unique_ptr<OniaSkimmer>(new OniaSkimmerReadData(tree,oniaCutter,outTreeName));
    } 
}
        


template class OniaSkimmerBase<OniaReaderMC>;
template class OniaSkimmerBase<OniaReaderRealData>;

