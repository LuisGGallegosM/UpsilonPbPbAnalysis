#ifndef TREEREADER
#define TREEREADER

#include"TTree.h"

class TreeReader
{
    private:
    std::vector<TBranch*> input_branches;
    TTree* tree_input;

    public:
    TreeReader(TTree* treeIn);

    void readTree(Long64_t index);
    void addInput(const char* varName,void* address);
    Long64_t getEntriesNumber() const {return tree_input->GetEntries();}
    std::string getName() const {return tree_input->GetName();}
    TTree* getTree() {return tree_input;}
};

#endif