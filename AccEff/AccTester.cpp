
#include "AccTester.h"

AccEffTester::AccEffTester(TTree* treeIn,AccEffOutputer* outp , AccCutter* accCut, EffCutter* effCut)
: TreeProcessor(treeIn), accCutter(accCut),effCutter(effCut), outputer(outp)
{
    TBranch* branch;

    //input branches
    addInput("Reco_QQ_4mom",&dataIn.mom4_RecoQQ);
    addInput("Reco_mu_4mom",&dataIn.mom4_RecoMu);
    addInput("Reco_QQ_size",&dataIn.recoQQsize);
    addInput("Reco_mu_size",&dataIn.recoMuSize);
    addInput("Reco_QQ_mupl_idx",dataIn.mupl_idx);
    addInput("Reco_QQ_mumi_idx",dataIn.mumi_idx);
    addInput("Reco_mu_SelectionType",dataIn.SelectionType);
    addInput("Reco_mu_nTrkWMea",dataIn.nTrkWMea);
    addInput("Reco_mu_nPixWMea",dataIn.nPixWMea);
    addInput("Reco_mu_dxy",dataIn.dxy);
    addInput("Reco_mu_dz",dataIn.dz);
    addInput("Reco_QQ_VtxProb",dataIn.VtxProb);
    addInput("Reco_QQ_trig",dataIn.trig);
    addInput("Reco_QQ_sign",dataIn.sign);
    addInput("Reco_mu_whichGen",dataIn.RecoMuWhichGen);
    addInput("Gen_QQ_4mom",&dataIn.mom4_GenQQ);
    addInput("Gen_mu_4mom",&dataIn.mom4_GenMu);
    addInput("Gen_QQ_size",&dataIn.genQQsize);
    addInput("Gen_mu_size",&dataIn.genMuSize);
    addInput("Gen_QQ_mupl_idx",dataIn.genQQ_mupl_idx);
    addInput("Gen_QQ_mumi_idx",dataIn.genQQ_mumi_idx);
    addInput("Gen_QQ_momId",dataIn.GenQQid);
    
    std::cout << "Using " << accCutter->getName() <<" cutter.\n";
    std::cout << "Using " << effCutter->getName() <<" cutter.\n";
    return;
}

void AccEffTester::ProcessEvent(Long64_t entry)
{
    Long64_t size=dataIn.genQQsize;
    
    for(Long64_t i=0;i<size;++i)
    {
        outputer->WriteData(dataIn,i,entry,*accCutter,*effCutter);
    }
}