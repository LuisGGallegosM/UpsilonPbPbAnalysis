#ifndef OUTPUTER
#define OUTPUTER

#include "TTree.h"

#define MAXTREESIZE (25000000000)

class Outputer
{
    public:
    virtual void Write() = 0;

    virtual ~Outputer() {};
};

class TreeOutputer : public Outputer
{
    private:
    TTree* tree_output;
    std::vector<TBranch*> output_branches;

    protected:
    template<typename T>
    void addOutput(const char* varName,T* var)
    {
        TBranch* branch =tree_output->Branch(varName, var);
        if (branch==nullptr)
            throw std::runtime_error(std::string("Error: Branch ")+varName+" does not exist.\n");
        output_branches.push_back(branch);
    }

    void FillEntries(){ tree_output->Fill();}

    public:
    TreeOutputer(const char* treeOutName);

    void Write() override;

    TTree* GetTree() {return tree_output;}
};

#endif