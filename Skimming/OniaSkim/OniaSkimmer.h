
#ifndef ONIASKIMMER
#define ONIASKIMMER

#include "../../OniaBase/OniaIO/OniaIO.h"
#include "../../OniaBase/TreeProcessor/TreeProcessor.h"
#include "../Common/Skimmer.h"
#include "OniaCutter.h"

template<typename Reader>
class OniaSkimmerBase : public Skimmer
{
    private:
    OniaReader<Reader> oniaReader;
    std::unique_ptr<Cutter<Reader>> oniaCutter;
    OniaWriter<OniaQQ> oniaWriter;

    public:
    OniaSkimmerBase(TTree* tree , Cutter<Reader>* cutter, const char* outTreeName): 
       oniaCutter(cutter) ,oniaWriter(outTreeName,"reco_"), oniaReader(tree)
    {
    }

    void Write() override 
    { 
        oniaWriter.getTree()->Write();
    }

    void Skim() override 
    { 
        TreeProcess(this,oniaReader.getName(),oniaReader.getEntries());
    }

    void ProcessEvent(Long64_t entry)
    {
        auto input= oniaReader.getData(entry);
        int size=input->recoQQ.size;
        
        for(int iQQ=0;iQQ<size;++iQQ)
        {
            if (oniaCutter->cut(input,iQQ,entry))
            {
                writeRecoQQ(input,oniaWriter.getDataBuffer(),SimpleSelector{entry,iQQ});
                oniaWriter.writeData();
            }
        }
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