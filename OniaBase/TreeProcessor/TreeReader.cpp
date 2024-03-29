
#include"TreeReader.h"

TreeReader::TreeReader(TTree* treeIn) 
    :inputBranches(),inputTree(treeIn)
{
}

void TreeReader::readTree(Long64_t index) const
{
    for(TBranch* branch : inputBranches)
    {
        branch->GetEntry(index);
    }
}

void TreeReader::addInput(const char* name,void* address, const char* prefix)
{
    std::string varName(name);
    if (prefix!=nullptr) varName = prefix+varName;
    TBranch* branch = inputTree->GetBranch(varName.data());
    if (branch ==nullptr)
    {
        std::string error = std::string("Tried to read branch '")+varName+ "' does not exist.";
        throw std::runtime_error(error);
    }
    branch->SetAddress(address);
    inputBranches.push_back(branch);
}

bool TreeReader::addInputOpt(const char* varName,void* address)
{
    TBranch* branch = inputTree->GetBranch(varName);
    if (branch ==nullptr) return false;
    branch->SetAddress(address);
    inputBranches.push_back(branch);
    return true;
}

bool TreeReader::HasBranchInTree(const char* varName) const
{
    TBranch* branch = inputTree->GetBranch(varName);
    return (branch !=nullptr);
}

bool TreeReader::hasBranchInInputs(const char* varName) const
{
    bool result=false;
    const std::string name=varName;
    for(const TBranch* branch : inputBranches)
    {
        if(branch->GetName() == name)
        {
            result=true;
            break;
        }
    }
    return result;
}