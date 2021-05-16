#ifndef TREEPROCESSOR
#define TREEPROCESSOR

#include "TTree.h"
#include "TreeReader.h"
#include "TreeWriter.h"

#include <iostream>

template<typename TreeProcessor>
void TreeProcess(TreeProcessor processor, const std::string& treeName , Long64_t entries)
{
    int block =0;

    std::cout << "Processing of " << treeName <<" tree starting...\n";
    std::cout << entries <<" entries in tree\n";

    for(Long64_t i=0;i<entries;++i)
    {
        if ((i % (entries/20)) ==0)
        {
            std::cout <<"Processing: " << block*5 << "%" << std::endl;
            ++block;
        }
        processor->ProcessEvent(i);
    }
    std::cout << "Total readed entries " << entries;
    std::cout << " from '" << treeName << "' tree\n";
    return;
}

#endif