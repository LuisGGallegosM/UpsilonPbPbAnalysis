
#include "OniaDataSimple.h"

//OniaSimpleInfo

void OniaSimpleInfo::addOutputs(TreeWriter* writer)
{
    writer->addOutput("Evt",&Evt);
    writer->addOutput("pdfId",&pdgId);
}

//OniaSimpleQQ

void OniaSimpleQQ::addOutputs(TreeWriter* writer,const char* prefix)
{
    writer->addOutput("mass",&mass,prefix);
    writer->addOutput("eta",&eta,prefix);
    writer->addOutput("phi",&phi,prefix);
    writer->addOutput("pT",&pT,prefix);
    writer->addOutput("y",&y,prefix);
}

void OniaSimpleQQ::Write(TLorentzVector* dimuon)
{
    mass= dimuon->M();
    pT = dimuon->Pt();
    y = dimuon->Rapidity();
    phi = dimuon->Phi();
    eta = dimuon->Eta();
}

void OniaSimpleExtraQQ::addOutputs(TreeWriter* writer,const char* prefix)
{
    OniaSimpleQQ::addOutputs(writer,prefix);
    writer->addOutput("l",&l,prefix);
}

void OniaSimpleExtraQQ::Write(TLorentzVector* dimuon, float ctau)
{
    OniaSimpleQQ::Write(dimuon);
    l = ctau;
}

//OniaSimpleMu

void OniaSimpleMu::Write(TLorentzVector* muonPl, TLorentzVector* muonMi)
{
    pT_mi = muonMi->Pt();
    eta_mi = muonMi->Eta();
    phi_mi = muonMi->Phi();
    pT_pl = muonPl->Pt();
    eta_pl = muonPl->Eta();
    phi_pl = muonPl->Phi();
}

void OniaSimpleMu::addOutputs(TreeWriter* writer, const char* prefix)
{
    writer->addOutput("pT_mi",&pT_mi, prefix);
    writer->addOutput("pT_pl",&pT_pl, prefix);
    writer->addOutput("eta_mi",&eta_mi, prefix);
    writer->addOutput("eta_pl",&eta_pl, prefix);
    writer->addOutput("phi_mi",&phi_mi, prefix);
    writer->addOutput("phi_pl",&phi_pl, prefix);
}

void OniaSimpleJet::addOutputs(TreeWriter* writer)
{
    writer->addOutput("jt_pt",&jt_pt);
    writer->addOutput("jt_rap",&jt_rap);
    writer->addOutput("jt_eta",&jt_eta);
    writer->addOutput("jt_phi",&jt_phi);
    writer->addOutput("jt_pt_JEU_Down",&jt_pt_JEU_Down);
    writer->addOutput("z",&z);
    writer->addOutput("jt_pt_JEU_Up",&jt_pt_JEU_Up);
    writer->addOutput("jt_pt_jettyCorr",&jt_pt_jettyCorr);
    writer->addOutput("z_jettyCorr",&z_jettyCorr);
    writer->addOutput("jt_pt_noZJEC",&jt_pt_noZJEC);
}

void OniaSimpleRefJet::addOutputs(TreeWriter* writer)
{
    writer->addOutput("jt_ref_pt",&jt_ref_pt);
    writer->addOutput("jt_ref_rap",&jt_ref_rap);
    writer->addOutput("jt_ref_eta",&jt_ref_eta);
    writer->addOutput("jt_ref_phi",&jt_ref_phi);
    writer->addOutput("gen_z",&gen_z);
    writer->addOutput("jt_pt_genZJEC",&jt_pt_genZJEC);
    writer->addOutput("pthat",&pthat);
    writer->addOutput("accxeff",&accxeff);
}