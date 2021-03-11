
#include "OniaReader.h"

OniaReaderMC::OniaReaderMC(TTree* treeIn) : 
    reader(treeIn)
{
    recoQQ.addInputs(&reader);
    recoMu.addInputs(&reader);
    which.addInputs(&reader);
    genQQ.addInputs(&reader);
    genMu.addInputs(&reader);
}

OniaReaderRealData::OniaReaderRealData(TTree* treeIn) : 
    reader(treeIn)
{
    recoQQ.addInputs(&reader);
    recoMu.addInputs(&reader);
}

OniaReaderGenOnly::OniaReaderGenOnly(TTree* treeIn) : 
    reader(treeIn)
{
    genQQ.addInputs(&reader);
    genMu.addInputs(&reader);
}

OniaJetReaderMC::OniaJetReaderMC(TTree* treeIn) : 
    reader(treeIn)
{
    recoQQ.addInputs(&reader);
    recoMu.addInputs(&reader);
    which.addInputs(&reader);
    genQQ.addInputs(&reader);
    genMu.addInputs(&reader);
    jetInfo.addInputs(&reader);
}

OniaJetReaderRealData::OniaJetReaderRealData(TTree* treeIn) : 
    reader(treeIn)
{
    recoQQ.addInputs(&reader);
    recoMu.addInputs(&reader);
}