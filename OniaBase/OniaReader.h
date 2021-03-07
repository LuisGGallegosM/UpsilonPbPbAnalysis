
#ifndef ONIADATA
#define ONIADATA

#include "TClonesArray.h"
#include "../TreeProcessor/TreeReader.h"
#include "OniaData/OniaDataQQ.h"

class OniaReader
{
    private:
    TreeReader reader;
    
    public:
    OniaGenQQ genQQ;
    OniaRecoQQ recoQQ;
    OniaGenMu genMu;
    OniaRecoMu recoMu;
    OniaWhich which;

    OniaReader(TTree* treeIn);
    const TreeReader* getReader() const {return &reader;}
};

class OniaReader2
{
    private:
    TreeReader reader;
    public:
    OniaGenQQ2 genQQ;
    OniaGenMu genMu;
    OniaReader2(TTree* treeIn);
    const TreeReader* getReader() const {return &reader;}
};

#endif