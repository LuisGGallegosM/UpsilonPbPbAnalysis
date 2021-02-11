#ifndef TREEPROCESSOR
#define TREEPROCESSOR

#include "TTree.h"

class TreeProcessor
{
    private:
    std::vector<TBranch*> input_branches;
    TTree* tree_input;

    void GetEntries(Long64_t index);

    protected:
    virtual void ProcessEvent(Long64_t entry) = 0;
    
    void addInput(const char* varName,void* address);
    void Process();

    public:
    TreeProcessor(TTree* treeIn);
    
    virtual ~TreeProcessor() {}
};

#endif