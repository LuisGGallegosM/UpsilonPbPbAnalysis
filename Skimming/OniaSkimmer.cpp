

#include "OniaSkimmer.h"
#include "TLorentzVector.h"

OniaSkimmer::OniaSkimmer(TTree* treeIn, OniaOutputer* outp , OniaCutter* cut) 
: TreeProcessor(treeIn), cutter(cut), outputer(outp)
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

    bool dataIsMC=(treeIn->GetBranch("Reco_mu_whichGen")!=nullptr);
    if (cut->isMC() != dataIsMC)
    {
        throw std::invalid_argument("ERROR : is MC cut and data mismatch\n");
    }

    //MC only branch check
    if(cut->isMC())
    {  
        addInput("Reco_mu_whichGen",dataIn.RecoMuWhichGen);
        addInput("Gen_mu_whichReco",dataIn.GenMuWhichReco);
        addInput("Gen_QQ_4mom",&dataIn.mom4_GenQQ);
        addInput("Gen_mu_4mom",&dataIn.mom4_GenMu);
        addInput("Gen_QQ_size",&dataIn.genQQsize);
        addInput("Gen_mu_size",&dataIn.genMuSize);
        addInput("Gen_QQ_mupl_idx",dataIn.genQQ_mupl_idx);
        addInput("Gen_QQ_mumi_idx",dataIn.genQQ_mumi_idx);
        addInput("Gen_QQ_momId",dataIn.GenQQid);
    }
    return;
}

void OniaSkimmer::ProcessEvent(Long64_t entry)
{
    if (cutter->prescale(entry)) return;
    Long64_t size=dataIn.getSizeRecoQQ();
    
    for(Long64_t i=0;i<size;++i)
    {
        if (cutter->cut(&dataIn,i,entry))
        {
            outputer->WriteData(dataIn,i,entry);
        }
    }
}