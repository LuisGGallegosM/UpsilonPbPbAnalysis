
#include "AccTester.h"

AccEffTester::AccEffTester(TTree* treeIn,AccEffOutputer* outp , AccCutter* accCut, EffCutter* effCut)
: TreeProcessor(treeIn), accCutter(accCut),effCutter(effCut), outputer(outp)
{
    TBranch* branch;

    //input branches
    addInput("Reco_QQ_4mom",&dataIn.recoQQ.mom4);
    addInput("Reco_QQ_size",&dataIn.recoQQ.size);
    addInput("Reco_QQ_mupl_idx",dataIn.recoQQ.mupl_idx);
    addInput("Reco_QQ_mumi_idx",dataIn.recoQQ.mumi_idx);
    addInput("Reco_QQ_VtxProb",dataIn.VtxProb);
    addInput("Reco_QQ_trig",dataIn.trig);
    addInput("Reco_QQ_sign",dataIn.recoQQ.sign);

    addInput("Reco_mu_4mom",&dataIn.recoMu.mom4);
    addInput("Reco_mu_size",&dataIn.recoMu.size);
    addInput("Reco_mu_SelectionType",dataIn.SelectionType);
    addInput("Reco_mu_nTrkWMea",dataIn.nTrkWMea);
    addInput("Reco_mu_nPixWMea",dataIn.nPixWMea);
    addInput("Reco_mu_dxy",dataIn.dxy);
    addInput("Reco_mu_dz",dataIn.dz);
    addInput("Reco_mu_whichGen",dataIn.RecoMuWhichGen);

    addInput("Gen_QQ_4mom",&dataIn.genQQ.mom4);
    addInput("Gen_QQ_size",&dataIn.genQQ.size);
    addInput("Gen_QQ_mupl_idx",dataIn.genQQ.mupl_idx);
    addInput("Gen_QQ_mumi_idx",dataIn.genQQ.mumi_idx);
    addInput("Gen_QQ_momId",dataIn.GenQQid);
    addInput("Gen_QQ_whichRec",dataIn.GenQQWhichReco);

    addInput("Gen_mu_4mom",&dataIn.genMu.mom4);
    addInput("Gen_mu_size",&dataIn.genMu.size);
    
    std::cout << "Using " << accCutter->getName() <<" cutter.\n";
    std::cout << "Using " << effCutter->getName() <<" cutter.\n";
    return;
}

void AccEffTester::ProcessEvent(Long64_t entry)
{
    Long64_t size=dataIn.genQQ.size;
    
    for(Long64_t i=0;i<size;++i)
    {
        outputer->WriteData(dataIn,i,entry,*accCutter,*effCutter);
    }
}