
#ifndef ONIADATA
#define ONIADATA

#include "../TreeProcessor/TreeProcessor.h"
#include "OniaDataQQ.h"
#include "TClonesArray.h"

struct OniaRealData
{
    OniaRecoQQ recoQQ;
    OniaRecoMu recoMu;
};

struct OniaMCData
{
    OniaRecoQQ recoQQ;
    OniaRecoMu recoMu;
    OniaGenQQ genQQ;
    OniaGenMu genMu;
    OniaWhich which;
};

struct OniaGenOnlyData
{
    OniaGenQQ2 genQQ;
    OniaGenMu genMu;
};

struct OniaJetMCData
{
    OniaRecoQQ recoQQ;
    OniaRecoMu recoMu;
    OniaGenQQ genQQ;
    OniaGenMu genMu;
    OniaWhich which;
    OniaJetInfo jetInfo;
    OniaJetRef jetRef;
};

struct OniaJetRealData
{
    OniaRecoQQ recoQQ;
    OniaRecoMu recoMu;
    OniaJetInfo jetInfo;
};

void addInputs(OniaMCData* data ,TreeReader* reader);
void addInputs(OniaRealData* data ,TreeReader* reader);
void addInputs(OniaGenOnlyData* data ,TreeReader* reader);
void addInputs(OniaJetMCData* data ,TreeReader* reader);
void addInputs(OniaJetRealData* data ,TreeReader* reader);

template<typename Data>
class OniaReader
{
    Data data;
    TreeReader reader;
    public:
    OniaReader(TTree* tree) : reader(tree)
    {
        addInputs(&data,&reader);
    }
    const Data* getData(Long64_t entry) 
    { 
        reader.readTree(entry);
        return &data;
    }
    std::string getName() { return reader.getName();}
    int getEntries() { return reader.getEntriesNumber();}
};


#endif