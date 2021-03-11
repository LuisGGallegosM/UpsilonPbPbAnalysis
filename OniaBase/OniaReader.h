
#ifndef ONIADATA
#define ONIADATA

#include "TClonesArray.h"
#include "../TreeProcessor/TreeReader.h"
#include "OniaData/OniaDataQQ.h"

class OniaReaderRealData
{
    private:
    TreeReader reader;
    
    public:
    OniaRecoQQ recoQQ;
    OniaRecoMu recoMu;

    OniaReaderRealData(TTree* treeIn);
    const TreeReader* getReader() const {return &reader;}
};

class OniaReaderMC
{
    private:
    TreeReader reader;   
    public:
    OniaRecoQQ recoQQ;
    OniaRecoMu recoMu;
    OniaGenQQ genQQ;
    OniaGenMu genMu;
    OniaWhich which;

    OniaReaderMC(TTree* treeIn);
    const TreeReader* getReader() const {return &reader;}
};

class OniaReaderGenOnly
{
    private:
    TreeReader reader;
    public:
    OniaGenQQ2 genQQ;
    OniaGenMu genMu;
    OniaReaderGenOnly(TTree* treeIn);
    const TreeReader* getReader() const {return &reader;}
};

class OniaJetReaderMC
{
    private:
    TreeReader reader;   
    public:
    OniaRecoQQ recoQQ;
    OniaRecoMu recoMu;
    OniaGenQQ genQQ;
    OniaGenMu genMu;
    OniaWhich which;
    OniaJetInfo jetInfo;

    OniaJetReaderMC(TTree* treeIn);
    const TreeReader* getReader() const {return &reader;} 
};

class OniaJetReaderRealData
{
    private:
    TreeReader reader;   
    public:
    OniaRecoQQ recoQQ;
    OniaRecoMu recoMu;

    OniaJetReaderRealData(TTree* treeIn);
    const TreeReader* getReader() const {return &reader;} 
};

#endif