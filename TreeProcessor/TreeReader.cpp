
#include"TreeReader.h"

TreeReader::TreeReader(TTree* treeIn) 
    :tree_input(treeIn) ,input_branches()
{
}

void TreeReader::readTree(Long64_t index)
{
    for(TBranch* branch : input_branches)
    {
        branch->GetEntry(index);
    }
}

void TreeReader::addInput(const char* varName,void* address)
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

bool TreeReader::addInputOpt(const char* varName,void* address)
{
    TBranch* branch = tree_input->GetBranch(varName);
    if (branch ==nullptr) return false;
    branch->SetAddress(address);
    input_branches.push_back(branch);
    return true;
}

bool TreeReader::hasBranch(const char* varName) const
{
    bool result=false;
    const std::string name=varName;
    for(const TBranch* branch : input_branches)
    {
        if(branch->GetName() == name)
        {
            result=true;
            break;
        }
    }
    return result;
}