
#include "TreeProcessor.h"
#include <iostream>

void TreeProcessor::Process()
{
    int block =0;
    Long64_t entries= reader.getEntriesNumber();

    std::cout << "Processing of " << reader.getName() <<" tree starting...\n";
    std::cout << entries <<" entries in tree\n";

    for(Long64_t i=0;i<entries;++i)
    {
        if ((i % (entries/20)) ==0)
        {
            std::cout <<"Processing: " << block*5 << "%" << std::endl;
            ++block;
        }
        reader.readTree(i);
        ProcessEvent(i);
    }
    std::cout << "Total readed entries " << entries;
    std::cout << " from '" << reader.getName() << "' tree\n";
    return;
}