
#include "TreeProcessor.h"
#include <iostream>

void TreeProcessor::Process(const TreeReader* treeReader)
{
    int block =0;
    Long64_t entries= treeReader->getEntriesNumber();

    std::cout << "Processing of " << treeReader->getName() <<" tree starting...\n";
    std::cout << entries <<" entries in tree\n";

    for(Long64_t i=0;i<entries;++i)
    {
        if ((i % (entries/20)) ==0)
        {
            std::cout <<"Processing: " << block*5 << "% \n";
            ++block;
        }
        treeReader->readTree(i);
        ProcessEvent(i);
    }
    std::cout << "Total readed entries " << entries;
    std::cout << " from '" << treeReader->getName() << "' tree\n";
    return;
}