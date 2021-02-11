
#include "Outputer.h"
#include <iostream>

//OniaOutputerTree
TreeOutputer::TreeOutputer(const char* treeOutName)
{
    tree_output= new TTree(treeOutName, "Skimmed tree");
    tree_output->SetMaxTreeSize(MAXTREESIZE);
}

void TreeOutputer::Write() 
{
    std::cout << "Total output entries " << tree_output->GetEntries();
    std::cout << " to '" << tree_output->GetName() << "' tree\nDone.\n";
    tree_output->Write(0,TObject::kOverwrite); 
}