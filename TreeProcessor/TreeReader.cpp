
#include"TreeReader.h"

TreeReaderBase::TreeReaderBase(TTree* treeIn) 
    :tree_input(treeIn) ,input_branches()
{
}

void TreeReaderBase::readTree(Long64_t index)
{
    for(int i=0 ;i<input_branches.size();i++)
    {
        input_branches[i]->GetEntry(index);
    }
}

void TreeReaderBase::addInput(const char* varName,void* address)
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