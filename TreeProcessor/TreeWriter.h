#ifndef TREEWRITER
#define TREEWRITER

#include "TTree.h"

#define MAXTREESIZE (25000000000)

class TreeWriter
{
    private:
    TTree* tree_output;
    std::vector<TBranch*> output_branches;

    public:
    TreeWriter(const char* treeOutName);

    template<typename T>
    void addOutput(const char* varName,T* var)
    {
        TBranch* branch =tree_output->Branch(varName, var);
        if (branch==nullptr)
            throw std::runtime_error(std::string("Error: Branch ")+varName+" does not exist.\n");
        output_branches.push_back(branch);
    }

    void FillEntries() { tree_output->Fill();}
    void Write();
    TTree* getTree() {return tree_output;}
};

#endif