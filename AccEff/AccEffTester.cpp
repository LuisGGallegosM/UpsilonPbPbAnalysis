
#include "AccEffTester.h"

AccEffTester::AccEffTester(TTree* treeIn,AccEffOutputer* outp)
: TreeProcessor(), dataIn(treeIn,true), outputer(outp)
{
}

void AccEffTester::ProcessEvent(Long64_t entry)
{
    Long64_t size=dataIn.genQQ.size;
    
    for(Long64_t i=0;i<size;++i)
    {
        outputer->WriteData(dataIn,i,entry);
    }
}