
#ifndef ONIASKIMMER
#define ONIASKIMMER

#include "../../TreeProcessor/TreeProcessor.h"
#include "../../OniaBase/OniaIO/OniaIO.h"
#include "../Skimmer.h"
#include "OniaCutter.h"

template<typename Reader>
class OniaSkimmerBase : public TreeProcessor, public Skimmer
{
    private:
    Reader oniaReader;
    std::unique_ptr<Cutter<Reader>> oniaCutter;
    OniaWriterRecoQQ oniaWriter;
    void ProcessEvent(Long64_t entry) override
    {
        Long64_t size=oniaReader.recoQQ.size;
        
        for(Long64_t iQQ=0;iQQ<size;++iQQ)
        {
            if (oniaCutter->cut(&oniaReader,iQQ,entry))
            {
                oniaWriter.writeData(&oniaReader,iQQ,entry);
                FillEntries();
            }
        }
    }

    public:
    OniaSkimmerBase(TTree* tree , Cutter<Reader>* cutter, const char* outTreeName): 
        TreeProcessor(tree,outTreeName),oniaCutter(cutter) ,oniaWriter(), oniaReader()
    {
        registerInputs(&oniaReader);
        registerOutputs(&oniaWriter);
    }
    void Write() override 
    { 
        TreeProcessor::write();
    }
    void Skim() override 
    { 
        Process(); 
    }
};

using OniaSkimmerMC = OniaSkimmerBase<OniaMCData>;
using OniaSkimmerReadData = OniaSkimmerBase<OniaRealData>;

std::unique_ptr<Skimmer> createOniaSkimmer(TTree* tree ,const CutParams* cutter, const char* outTreeName);

#if defined(__CLING__)
#include "OniaSkimmer.cpp"
#endif

#endif