#pragma once

#include "TTree.h"
#include "TLorentzVector.h"
#include "TString.h"
#include "TClonesArray.h"
#include <new>
#include <functional>
#include "TTree.h"

#define maxBranchSize (1000)
#define MAXTREESIZE (100000000)

template <class T,class U>
class Skimmer
{
    protected:
    std::vector<TBranch*> input_branches;
    T dataIn;
    U dataOut;
    TTree* tree;
    TTree* tree_output;

    virtual void WriteData(Int_t index) = 0;

    void GetEntries(Long64_t index)
    {
        for(int i=0 ;i<input_branches.size();i++)
        {
            input_branches[i]->GetEntry(index);
        }
        return;
    }
    
    public:
    Skimmer(TTree* myTree)
    {
        tree = myTree;
        TString name(tree->GetName());
        tree_output= new TTree(name + "_Skimmed", "Skimmed tree");
        tree_output->SetMaxTreeSize(MAXTREESIZE);
    }

    void Skim(std::function<bool(T*,Int_t)> cutter)
    {
        int block =0;
        Long64_t entries= tree->GetEntries();

        printf("Onia Skimming start...");
        printf("%lld entries from '%s' tree\n", entries, tree->GetName());

        for(Long64_t i=0;i<entries;++i)
        {
            if ((i % (entries/20)) ==0)
            {
                printf("Processing: %i%%\n",block*5);
                ++block;
            }
                
            GetEntries(i);

            for(Long64_t j=0;j<dataIn.size;++j)
            {
                if (cutter(&dataIn,j))
                {
                    WriteData(j);
                    tree_output->Fill();
                }
            }

        }
        printf("Total readed entries %lld from '%s' tree\n", entries, tree->GetName());
        printf("Total output entries %lld to '%s' tree\n", tree_output->GetEntries(),tree_output->GetName());
        printf("..Saving tree\n");
        return;
    }
    TTree* GetTree()
    {
        return tree_output;
    }
};