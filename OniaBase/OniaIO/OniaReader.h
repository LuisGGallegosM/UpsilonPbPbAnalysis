
#ifndef ONIADATA
#define ONIADATA

#include "../../TreeProcessor/TreeProcessor.h"
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
};

void addInputs(OniaMCData* data ,TreeReader* reader);
void addInputs(OniaRealData* data ,TreeReader* reader);
void addInputs(OniaGenOnlyData* data ,TreeReader* reader);
void addInputs(OniaJetMCData* data ,TreeReader* reader);
void addInputs(OniaJetRealData* data ,TreeReader* reader);

template<typename Data>
class OniaReader : public DataReader
{
    Data data;
    public:
    OniaReader() = default;

    void registerReader(TreeReader* reader) override
    {
        addInputs(&data,reader);
    }
    const Data* getData() { return &data;}
};


#endif