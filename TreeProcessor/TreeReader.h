#ifndef TREEREADER
#define TREEREADER

#include"TTree.h"

class TreeReader
{
    public:
    virtual void readTree(Long64_t index)=0;
    virtual void addInput(const char* varName,void* address)=0;
    virtual Long64_t getEntriesNumber() const=0;
    virtual std::string getName() const=0;
    virtual TTree* getTree() =0;

    virtual ~TreeReader() = default;
};

class TreeReaderBase : public TreeReader
{
    private:
    std::vector<TBranch*> input_branches;
    TTree* tree_input;

    public:
    TreeReaderBase(TTree* treeIn);

    void readTree(Long64_t index) override;
    void addInput(const char* varName,void* address) override;
    Long64_t getEntriesNumber() const override {return tree_input->GetEntries();}
    std::string getName() const override {return tree_input->GetName();}
    TTree* getTree() override {return tree_input;}
};

#endif