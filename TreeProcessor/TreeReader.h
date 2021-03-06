#ifndef TREEREADER
#define TREEREADER

#include"TTree.h"

class TreeReader
{
    private:
    std::vector<TBranch*> inputBranches;
    TTree* inputTree;

    public:
    TreeReader(TTree* treeIn);

    void readTree(Long64_t index) const;
    bool addInputOpt(const char* varName,void* address);
    void addInput(const char* varName,void* address);
    bool hasBranchInInputs(const char* varName) const;
    bool HasBranchInTree(const char* varName) const;
    Long64_t getEntriesNumber() const {return inputTree->GetEntries();}
    std::string getName() const {return inputTree->GetName();}
    TTree* getTree() {return inputTree;}
};

#endif