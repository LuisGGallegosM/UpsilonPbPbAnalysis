
#ifndef ONIASKIMMER
#define ONIASKIMMER

#include "../../TreeProcessor/TreeProcessor.h"
#include "../../OniaBase/OniaReader.h"
#include "../../OniaBase/OniaWriter.h"
#include "../Skimmer.h"
#include "OniaCutter.h"

template<typename Reader>
class OniaSkimmerBase : public TreeProcessor, public Skimmer
{
    private:
    std::map<std::string,int> pdgIds;
    Reader oniaReader;
    std::unique_ptr<Cutter<Reader>> oniaCutter;
    OniaWriterReco<Reader> oniaWriter;
    void ProcessEvent(Long64_t entry) override;

    public:
    OniaSkimmerBase(TTree* tree , Cutter<Reader>* cutter, const char* outTreeName);
    void Write() override { oniaWriter.Write(); }
    void Skim() override { Process(oniaReader.getReader()); }
};

using OniaSkimmerMC = OniaSkimmerBase<OniaReaderMC>;
using OniaSkimmerReadData = OniaSkimmerBase<OniaReaderRealData>;

std::unique_ptr<Skimmer> createSkimmer(TTree* tree ,const CutParams* cutter, const char* outTreeName);

#if defined(__CLING__)
#include "OniaSkimmer.cpp"
#endif

#endif