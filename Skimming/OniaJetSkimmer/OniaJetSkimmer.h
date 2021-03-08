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
    void ProcessEvent(Long64_t entry) override;

    public:
    OniaJetSkimmer(TTree* tree , Cutter<Reader>* cutter, const char* outTreeName);
    void Write() override { oniaWriter.Write(); }
    void Skim() override { Process(oniaReader.getReader()); }
};

using OniaJetSkimmerMC = OniaJetSkimmer<OniaJetReaderMC>;
using OniaJetSkimmerRealData = OniaJetSkimmer<OniaJetReaderRealData>;

std::unique_ptr<Skimmer> createSkimmer(TTree* tree ,const CutParams* cutter, const char* outTreeName);


#endif