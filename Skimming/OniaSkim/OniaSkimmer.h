
#ifndef ONIASKIMMER
#define ONIASKIMMER

#include "../../OniaBase/OniaIO/OniaIO.h"
#include "../../OniaBase/TreeProcessor/TreeProcessor.h"
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
        int size=input->recoQQ.size;
        
        for(int iQQ=0;iQQ<size;++iQQ)
        {
            if (oniaCutter->cut(input,iQQ,entry))
            {
                oniaWriter.writeData(input,SimpleSelector{entry,iQQ});
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

/**
 * @brief Create a Onia Skimmer object
 * 
 * @param tree Tree to read
 * @param cutter Quality cut parameters
 * @param outTreeName Root output file name where to save skim.
 * @return std::unique_ptr<Skimmer>
 */
std::unique_ptr<Skimmer> createOniaSkimmer(TTree* tree ,const CutParams* cutter, const char* outTreeName);

#if defined(__CLING__)
#include "OniaSkimmer.cpp"
#endif

#endif