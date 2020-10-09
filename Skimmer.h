#pragma once

#include "TTree.h"
#include "TLorentzVector.h"
#include "TString.h"
#include "TClonesArray.h"
#include <new>
#include <functional>
#include "TTree.h"

#define maxBranchSize (1000)
#define MAXTREESIZE (10000000000)

template <class T,class U>
class Skimmer
{
    private:
    std::vector<TBranch*> input_branches;
    std::vector<TBranch*> output_branches;
    TTree* tree_input;
    TTree* tree_output;

    void GetEntries(Long64_t index)
    {
        for(int i=0 ;i<input_branches.size();i++)
        {
            input_branches[i]->GetEntry(index);
        }
        return;
    }

    void FillEntries()
    {
        tree_output->Fill();
        return;
    }

    protected:
    T dataIn;
    U dataOut;

    virtual void WriteData(Int_t index, Long64_t entry) = 0;

    public:
    Skimmer(TTree* treeIn, const char* treeOutName) 
    :tree_input(treeIn) ,input_branches(), output_branches()
    {
        TString name(tree_input->GetName());
        tree_output= new TTree(treeOutName, "Skimmed tree");
        tree_output->SetMaxTreeSize(MAXTREESIZE);
    }

    TTree* Skim(std::function<bool(T*,Int_t)> cutter)
    {
        int block =0;
        Long64_t entries= tree_input->GetEntries();

        printf("Skimming of '%s' tree starting...\n",tree_input->GetName());
        printf("%lld entries in tree\n", entries);

        for(Long64_t i=0;i<entries;++i)
        {
            if ((i % (entries/20)) ==0)
            {
                printf("Processing: %i%%\n",block*5);
                ++block;
            }
                
            GetEntries(i);

            for(Long64_t j=0;j<dataIn.getSize();++j)
            {
                if (cutter(&dataIn,j))
                {
                    WriteData(j,i);
                    FillEntries();
                }
            }

        }
        printf("Total readed entries %lld from '%s' tree\n", entries, tree_input->GetName());
        printf("Total output entries %lld to '%s' tree\nDone.\n", tree_output->GetEntries(),tree_output->GetName());
        return tree_output;
    }
    TTree* GetTree()
    {
        return tree_output;
    }

    template<class W>
    void addOutput(const char* varName, W* var)
    {
        TBranch* branch =tree_output->Branch(varName, var);
        output_branches.push_back(branch);
    }

    void addInput(const char* varName,void* address)
    {
        TBranch* branch = tree_input->GetBranch(varName);
        branch->SetAddress(address);
        input_branches.push_back(branch);
    }
};