
#include "OniaReader.h"

OniaGenQQ::OniaGenQQ(TreeReader* reader)
{
    genQQ_mom4.reset(new TClonesArray("TLorentzVector"));
    reader->addInput("Gen_QQ_4mom",&genQQ_mom4);
    reader->addInput("Gen_QQ_size",&genQQ_size);
    reader->addInput("Gen_QQ_mupl_idx",genQQ_mupl_idx);
    reader->addInput("Gen_QQ_mumi_idx",genQQ_mumi_idx);
    reader->addInput("Gen_QQ_momId",genQQ_id);
}

OniaGenQQ2::OniaGenQQ2(TreeReader* reader)
{
    genQQ_mom4.reset(new TClonesArray("TLorentzVector"));
    genQQ_mumi_mom4.reset(new TClonesArray("TLorentzVector"));
    genQQ_mupl_mom4.reset(new TClonesArray("TLorentzVector"));
    reader->addInput("Gen_QQ_4mom",&genQQ_mom4);
    reader->addInput("Gen_QQ_size",&genQQ_size);
    reader->addInput("Gen_QQ_mupl_4mom",&genQQ_mupl_mom4);
    reader->addInput("Gen_QQ_mumi_4mom",&genQQ_mumi_mom4);
    reader->addInput("Gen_QQ_momId",genQQ_id);
}

OniaRecoQQ::OniaRecoQQ(TreeReader* reader)
{
    recoQQ_mom4.reset(new TClonesArray("TLorentzVector"));
    reader->addInput("Reco_QQ_4mom",&recoQQ_mom4);
    reader->addInput("Reco_QQ_size",&recoQQ_size);
    reader->addInput("Reco_QQ_mupl_idx",recoQQ_mupl_idx);
    reader->addInput("Reco_QQ_mumi_idx",recoQQ_mumi_idx);
    reader->addInput("Reco_QQ_VtxProb",recoQQ_VtxProb);
    reader->addInput("Reco_QQ_trig",recoQQ_trig);
    reader->addInput("Reco_QQ_sign",recoQQ_sign);
}

OniaRecoMu::OniaRecoMu(TreeReader* reader)
{
    recoMu_mom4.reset(new TClonesArray("TLorentzVector"));
    reader->addInput("Reco_mu_4mom",&recoMu_mom4);
    reader->addInput("Reco_mu_size",&recoMu_size);
    reader->addInput("Reco_mu_SelectionType",recoMu_SelectionType);
    reader->addInput("Reco_mu_nTrkWMea",recoMu_nTrkWMea);
    reader->addInput("Reco_mu_nPixWMea",recoMu_nPixWMea);
    reader->addInput("Reco_mu_dxy",recoMu_dxy);
    reader->addInput("Reco_mu_dz",recoMu_dz);
}

OniaGenMu::OniaGenMu(TreeReader* reader)
{
    genMu_mom4.reset(new TClonesArray("TLorentzVector"));
    reader->addInput("Gen_mu_4mom",&genMu_mom4);
    reader->addInput("Gen_mu_size",&genMu_size);
}

OniaWhich::OniaWhich(TreeReader* reader)
{
    reader->addInput("Reco_mu_whichGen",RecoMuWhichGen);
    reader->addInput("Gen_mu_whichRec",GenMuWhichReco);
    reader->addInput("Gen_QQ_whichRec",GenQQWhichReco);
    reader->addInput("Reco_QQ_whichGen",RecoQQWhichGen);
}

OniaReader::OniaReader(TTree* treeIn) : 
    TreeReader(treeIn), OniaGenQQ(this), OniaRecoQQ(this),OniaGenMu(this),OniaRecoMu(this),OniaWhich(this)
{
}

OniaReader2::OniaReader2(TTree* treeIn) : 
    TreeReader(treeIn), OniaGenQQ2(this), OniaGenMu(this)
{
}