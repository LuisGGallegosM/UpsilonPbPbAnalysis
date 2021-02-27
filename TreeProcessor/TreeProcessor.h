#ifndef TREEPROCESSOR
#define TREEPROCESSOR

#include "TTree.h"
#include "TreeReader.h"

class TreeProcessor
{
    protected:
    virtual void ProcessEvent(Long64_t entry) = 0;
    void Process(TreeReader* treeReader);

    public:
    TreeProcessor() = default;
    
    virtual ~TreeProcessor() =default;
};

#endif