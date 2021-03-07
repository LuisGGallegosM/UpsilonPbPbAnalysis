
#include "OniaReader.h"

OniaReader::OniaReader(TTree* treeIn) : 
    reader(treeIn)
{
    genQQ.addInputs(&reader);
    recoQQ.addInputs(&reader);
    genMu.addInputs(&reader);
    recoMu.addInputs(&reader);
    which.addInputs(&reader);
}

OniaReader2::OniaReader2(TTree* treeIn) : 
    reader(treeIn)
{
    genQQ.addInputs(&reader);
    genMu.addInputs(&reader);
}