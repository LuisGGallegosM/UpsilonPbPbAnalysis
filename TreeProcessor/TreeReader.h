#ifndef TREEREADER
#define TREEREADER

#include"TTree.h"

class TreeReader
{
    struct Branch
    {
        std::string name;

    };
    private:
    std::vector<TBranch*> input_branches;
    TTree* tree_input;

    public:
    TreeReader(TTree* treeIn);

    void readTree(Long64_t index);
    bool addInputOpt(const char* varName,void* address);
    void addInput(const char* varName,void* address);
    bool hasBranch(const char* varName) const;
    Long64_t getEntriesNumber() const {return tree_input->GetEntries();}
    std::string getName() const {return tree_input->GetName();}
    TTree* getTree() {return tree_input;}
};

#endif