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
    void addOutput(const char* varName,T* var, const char* prefix=nullptr)
    {
        std::string name=varName;
        if (prefix!=nullptr) name=prefix +name;
        TBranch* branch =tree_output->Branch(name.data(), var);
        if (branch==nullptr)
            throw std::runtime_error(std::string("Error: Branch ")+name+" cannot be added.\n");
        output_branches.push_back(branch);
    }

    void FillEntries() { tree_output->Fill();}
    void Write();
    TTree* getTree() {return tree_output;}
};

#endif