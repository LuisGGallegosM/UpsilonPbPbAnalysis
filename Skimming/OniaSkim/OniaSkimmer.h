
#ifndef ONIASKIMMER
#define ONIASKIMMER

#include "../../TreeProcessor/TreeProcessor.h"
#include "../../OniaBase/OniaIO/OniaIO.h"
#include "../Common/Skimmer.h"
#include "OniaCutter.h"

template<typename Reader>
class OniaSkimmerBase : public TreeProcessor, public Skimmer
{
    private:
    OniaReader<Reader> oniaReader;
    std::unique_ptr<Cutter<Reader>> oniaCutter;
    OniaWriterRecoQQ oniaWriter;
    void ProcessEvent(Long64_t entry) override
    {
        auto input= oniaReader.getData();
        Long64_t size=input->recoQQ.size;
        
        for(Long64_t iQQ=0;iQQ<size;++iQQ)
        {
            if (oniaCutter->cut(input,iQQ,entry))
            {
                oniaWriter.writeData(input,iQQ,entry);
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