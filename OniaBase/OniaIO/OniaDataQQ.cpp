
#include"OniaDataQQ.h"

void OniaGenQQ::addInputs(TreeReader* reader)
{
    mom4.reset(new TClonesArray("TLorentzVector"));
    reader->addInput("Gen_QQ_4mom",&mom4);
    reader->addInput("Gen_QQ_size",&size);
    reader->addInput("Gen_QQ_mupl_idx",mupl_idx);
    reader->addInput("Gen_QQ_mumi_idx",mumi_idx);
    reader->addInput("Gen_QQ_momId",id);
}

void OniaGenQQ2::addInputs(TreeReader* reader)
{
    mom4.reset(new TClonesArray("TLorentzVector"));
    mumi_mom4.reset(new TClonesArray("TLorentzVector"));
    mupl_mom4.reset(new TClonesArray("TLorentzVector"));
    reader->addInput("Gen_QQ_4mom",&mom4);
    reader->addInput("Gen_QQ_size",&size);
    reader->addInput("Gen_QQ_mupl_4mom",&mupl_mom4);
    reader->addInput("Gen_QQ_mumi_4mom",&mumi_mom4);
    reader->addInput("Gen_QQ_momId",id);
}

void OniaRecoQQ::addInputs(TreeReader* reader)
{
    mom4.reset(new TClonesArray("TLorentzVector"));
    reader->addInput("Reco_QQ_4mom",&mom4);
    reader->addInput("Reco_QQ_size",&size);
    reader->addInput("Reco_QQ_mupl_idx",mupl_idx);
    reader->addInput("Reco_QQ_mumi_idx",mumi_idx);
    reader->addInput("Reco_QQ_VtxProb",VtxProb);
    reader->addInput("Reco_QQ_trig",trig);
    reader->addInput("Reco_QQ_sign",sign);
    reader->addInput("Reco_QQ_ctau", ctau);
}

void OniaRecoMu::addInputs(TreeReader* reader)
{
    mom4.reset(new TClonesArray("TLorentzVector"));
    reader->addInput("Reco_mu_4mom",&mom4);
    reader->addInput("Reco_mu_size",&size);
    reader->addInput("Reco_mu_SelectionType",SelectionType);
    reader->addInput("Reco_mu_nTrkWMea",nTrkWMea);
    reader->addInput("Reco_mu_nPixWMea",nPixWMea);
    reader->addInput("Reco_mu_dxy",dxy);
    reader->addInput("Reco_mu_dz",dz);
}

void OniaGenMu::addInputs(TreeReader* reader)
{
    mom4.reset(new TClonesArray("TLorentzVector"));
    reader->addInput("Gen_mu_4mom",&mom4);
    reader->addInput("Gen_mu_size",&size);
}

void OniaWhich::addInputs(TreeReader* reader)
{
    reader->addInput("Reco_mu_whichGen",RecoMuWhichGen);
    reader->addInput("Gen_mu_whichRec",GenMuWhichReco);
    reader->addInput("Gen_QQ_whichRec",GenQQWhichReco);
    reader->addInput("Reco_QQ_whichGen",RecoQQWhichGen);
}

void OniaJetInfo::addInputs(TreeReader* reader)
{
    reader->addInput("pBeamScrapingFilter",&pBeamScrapingFilter);
    reader->addInput("pPAprimaryVertexFilter",&pPAprimaryVertexFilter);
    reader->addInput("nref",&nref);
    reader->addInput("rawpt",rawpt);
    reader->addInput("jtpt",pt);
    reader->addInput("jteta",eta);
    reader->addInput("jty",y);
    reader->addInput("jtphi",phi);
    reader->addInput("jtarea",area);
    reader->addInput("jtm",m);
}

void OniaJetRef::addInputs(TreeReader* reader)
{
    reader->addInput("refpt",refpt);
    reader->addInput("refeta",refeta);
    reader->addInput("refy",refy);
    reader->addInput("refphi",refphi);
    reader->addInput("refm",refm);
    reader->addInput("refarea",refarea);
    reader->addInput("pthat",&pthat);
}