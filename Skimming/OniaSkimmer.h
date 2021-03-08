
#ifndef ONIASKIMMER
#define ONIASKIMMER

#include "../TreeProcessor/TreeProcessor.h"
#include "OniaCutter.h"
#include "../OniaBase/OniaReader.h"
#include "../OniaBase/OniaWriter.h"

class OniaSkimmer
{
    public:
    virtual void Write() = 0;
    virtual void Skim() = 0;

    virtual ~OniaSkimmer() = default;
};

template<typename Reader>
class OniaSkimmerBase : public TreeProcessor, public OniaSkimmer
{
    private:
    std::map<std::string,int> pdgIds;
    Reader oniaReader;
    std::unique_ptr<OniaCutter<Reader>> oniaCutter;
    OniaWriterReco<Reader> oniaWriter;
    void ProcessEvent(Long64_t entry) override;

    public:
    OniaSkimmerBase(TTree* tree , OniaCutter<Reader>* cutter, const char* outTreeName);
    void Write() override { oniaWriter.Write(); }
    void Skim() override { Process(oniaReader.getReader()); }
};

using OniaSkimmerMC = OniaSkimmerBase<OniaReaderMC>;
using OniaSkimmerReadData = OniaSkimmerBase<OniaReaderRealData>;

std::unique_ptr<OniaSkimmer> createSkimmer(TTree* tree ,const CutParams* cutter, const char* outTreeName);

#if defined(__CLING__)
#include "OniaSkimmer.cpp"
#endif

#endif