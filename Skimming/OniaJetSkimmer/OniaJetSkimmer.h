#ifndef ONIAJETSKIMMER
#define ONIAJETSKIMMER

#include "../../TreeProcessor/TreeProcessor.h"
#include "../Skimmer.h"
#include "../../OniaBase/OniaReader.h"
#include "../../OniaBase/OniaWriter.h"
#include "OniaJetCutter.h"

template<typename Reader>
class OniaJetSkimmer : public TreeProcessor, public Skimmer
{
    private:
    std::map<std::string,int> pdgIds;
    Reader oniaReader;
    std::unique_ptr<Cutter<Reader>> oniaCutter;
    OniaWriterReco<Reader> oniaWriter;
    void ProcessEvent(Long64_t entry) override
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

    public:
    OniaJetSkimmer(TTree* tree , Cutter<Reader>* cutter, const char* outTreeName): 
        oniaCutter(cutter) ,oniaReader(tree), oniaWriter(outTreeName)
    {
    }

    void Write() override 
    { 
        oniaWriter.Write(); 
    }
    
    void Skim() override 
    {
        Process(oniaReader.getReader()); 
    }
};

using OniaJetSkimmerMC = OniaJetSkimmer<OniaJetReaderMC>;
using OniaJetSkimmerRealData = OniaJetSkimmer<OniaJetReaderRealData>;

std::unique_ptr<Skimmer> createJetSkimmer(TTree* tree ,const CutParams* cutter, const char* outTreeName);


#endif