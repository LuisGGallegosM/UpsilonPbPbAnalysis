
#include "OniaReader.h"

OniaReader::OniaReader(TTree* treeIn) : 
    TreeReader(treeIn)
{
    recoMu_mom4.reset(new TClonesArray("TLorentzVector"));
    genMu_mom4.reset(new TClonesArray("TLorentzVector"));
    recoQQ_mom4.reset(new TClonesArray("TLorentzVector"));
    genQQ_mom4.reset(new TClonesArray("TLorentzVector"));
    genQQ_mumi_mom4.reset(new TClonesArray("TLorentzVector"));
    genQQ_mupl_mom4.reset(new TClonesArray("TLorentzVector"));

    //input branches
    addInput("Reco_QQ_4mom",&recoQQ_mom4);
    addInput("Reco_QQ_size",&recoQQ_size);
    addInput("Reco_QQ_mupl_idx",recoQQ_mupl_idx);
    addInput("Reco_QQ_mumi_idx",recoQQ_mumi_idx);
    addInput("Reco_QQ_VtxProb",VtxProb);
    addInput("Reco_QQ_trig",trig);
    addInput("Reco_QQ_sign",recoQQ_sign);

    addInput("Reco_mu_4mom",&recoMu_mom4);
    addInput("Reco_mu_size",&recoMu_size);
    addInput("Reco_mu_SelectionType",SelectionType);
    addInput("Reco_mu_nTrkWMea",nTrkWMea);
    addInput("Reco_mu_nPixWMea",nPixWMea);
    addInput("Reco_mu_dxy",dxy);
    addInput("Reco_mu_dz",dz);

    addInput("Gen_QQ_4mom",&genQQ_mom4);
    addInput("Gen_QQ_size",&genQQ_size);
    addInput("Gen_QQ_mupl_idx",genQQ_mupl_idx);
    addInput("Gen_QQ_mumi_idx",genQQ_mumi_idx);
    addInput("Gen_QQ_momId",genQQid);

    addInput("Gen_mu_4mom",&genMu_mom4);
    addInput("Gen_mu_size",&genMu_size);

    addInput("Reco_mu_whichGen",RecoMuWhichGen);
    addInput("Gen_mu_whichRec",GenMuWhichReco);
    addInput("Gen_QQ_whichRec",GenQQWhichReco);
    addInput("Reco_QQ_whichGen",RecoQQWhichGen);

    addInput("Gen_QQ_mupl_4mom",&genQQ_mupl_mom4);
    addInput("Gen_QQ_mumi_4mom",&genQQ_mumi_mom4);
}