#ifndef ONIAOUTPUT
#define ONIAOUTPUT

#include "TTree.h"
#include "Skimmer.h"
#include "OniaData.h"
#include "TH1.h"

class OniaOutputer
{
    public:
    virtual void WriteData(const Onia_Input& dataIn,Int_t index, Long64_t entry) = 0;
    virtual void Write() = 0;

    virtual ~OniaOutputer() {};
};

class OniaOutputerTree : public OniaOutputer
{
    private:
    TTree* tree_output;
    std::vector<TBranch*> output_branches;

    protected:
    void addOutput(const char* varName,Float_t* var);
    void addOutput(const char* varName,Int_t* var);

    public:
    OniaOutputerTree(const char* treeOutName);

    void Write() override;

    TTree* GetTree() {return tree_output;}
    void FillEntries(){ tree_output->Fill();}
    
};

class OniaOutputerQQ : public OniaOutputerTree
{
    private:
    Int_t Evt;
    Float_t mass;
    Float_t pT;
    Float_t y;
    Float_t phi;
    Float_t eta;
    Float_t pT_mi;
    Float_t eta_mi;
    Float_t phi_mi;
    Float_t pT_pl;
    Float_t eta_pl;
    Float_t phi_pl;

    public:
    OniaOutputerQQ(const char* treeOutName);

    void WriteData(const Onia_Input& dataIn,Int_t index, Long64_t entry) override;
};

class OniaOutputerMu : public OniaOutputerTree
{
    private:
    TH1F* hist;
    Int_t Evt;
    Float_t mass;
    Float_t pT;
    Float_t y;
    Float_t phi;
    Float_t eta;
    Float_t dR;

    public:
    OniaOutputerMu(const char* treeOutName);

    void WriteData(const Onia_Input& dataIn,Int_t index, Long64_t entry) override;
    void Write() override;
};

#endif