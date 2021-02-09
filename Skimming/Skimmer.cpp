
#include "Skimmer.h"

Skimmer::Skimmer(TTree* treeIn, const char* treeOutName) 
    :tree_input(treeIn) ,input_branches(), output_branches()
{
    TString name(tree_input->GetName());
    tree_output= new TTree(treeOutName, "Skimmed tree");
    tree_output->SetMaxTreeSize(MAXTREESIZE);
}

void Skimmer::GetEntries(Long64_t index)
{
    for(int i=0 ;i<input_branches.size();i++)
    {
        input_branches[i]->GetEntry(index);
    }
        return;
 }

void Skimmer::FillEntries()
{
    tree_output->Fill();
    return;
}

TTree* Skimmer::Skim()
{
    int block =0;
    Long64_t entries= tree_input->GetEntries();

    std::cout << "Skimming of " << tree_input->GetName() <<" tree starting...\n";
    std::cout << entries <<" entries in tree\n";

    for(Long64_t i=0;i<entries;++i)
    {
        if ((i % (entries/20)) ==0)
        {
            std::cout <<"Processing: " << block*5 << "% \n";
            ++block;
        }  
        GetEntries(i);
        ProcessEvent(i);
    }
    std::cout << "Total readed entries " << entries;
    std::cout << " from '" << tree_input->GetName() << "' tree\n";
    std::cout << "Total output entries " << tree_output->GetEntries();
    std::cout << " to '" << tree_output->GetName() << "' tree\nDone.\n";
    return tree_output;
}

void Skimmer::addOutput(const char* varName, Float_t* var)
{
    TBranch* branch =tree_output->Branch(varName, var);
    output_branches.push_back(branch);
}

void Skimmer::addOutput(const char* varName, Int_t* var)
{
    TBranch* branch =tree_output->Branch(varName, var);
    output_branches.push_back(branch);
}

void Skimmer::addInput(const char* varName,void* address)
{
    TBranch* branch = tree_input->GetBranch(varName);
    if (branch ==nullptr)
    {
        std::string error = std::string("Tried to read branch '")+varName+ "' does not exist.";
        throw std::runtime_error(error);
    }

    branch->SetAddress(address);
    input_branches.push_back(branch);
 }