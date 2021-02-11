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
    void addOutput(const char* varName,Float_t* var);
    void addOutput(const char* varName,Int_t* var);
    void FillEntries(){ tree_output->Fill();}

    public:
    TreeOutputer(const char* treeOutName);

    void Write() override;

    TTree* GetTree() {return tree_output;}
};

#endif