
#ifndef ONIASKIMMER
#define ONIASKIMMER

#include "../TreeProcessor/TreeProcessor.h"
#include "OniaCutter.h"
#include "../OniaBase/OniaReader.h"
#include "../OniaBase/OniaWriter.h"

class OniaSkimmer : public TreeProcessor
{
    private:
    std::map<std::string,int> pdgIds;
    std::unique_ptr<OniaReader> oniaReader;
    std::unique_ptr<OniaCutter> oniaCutter;
    std::unique_ptr<OniaWriterFull> oniaWriter;
    void ProcessEvent(Long64_t entry) override;

    public:
    OniaSkimmer(OniaReader* reader , OniaCutter* cutter, OniaWriterFull* writer);
    void Write() { oniaWriter->Write(); }
    void Skim() { Process(&(oniaReader->reader)); }
};

#if defined(__CLING__)
#include "OniaSkimmer.cpp"
#endif

#endif