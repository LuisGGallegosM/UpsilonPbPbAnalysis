
#include "OniaDataSimple.h"

//OniaSimpleInfo

void OniaSimpleInfo::addOutputs(TreeWriter* writer)
{
    writer->addOutput("Evt",&Evt);
    writer->addOutput("pdfId",&pdgId);
}

void OniaSimpleInfo::addInputs(TreeReader* reader)
{
    reader->addInput("Evt",&Evt);
    reader->addInput("pdfId",&pdgId);
}

//OniaSimpleQQ

void OniaSimpleQQ::addInputs(TreeReader* reader,const char* prefix)
{
    reader->addInput("pT",&pT,prefix);
    reader->addInput("eta",&eta,prefix);
    reader->addInput("y",&y,prefix);
    reader->addInput("phi",&phi,prefix);
    reader->addInput("mass",&mass,prefix);
}

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

//OniaSimpleExtraQQ

void OniaSimpleExtraQQ::addInputs(TreeReader* reader, const char* prefix)
{
    OniaSimpleQQ::addInputs(reader,prefix);
    reader->addInput("l",&l,prefix);
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

void OniaSimpleMu::addInputs(TreeReader* reader, const char* prefix)
{
    reader->addInput("pT_mi",&pT_mi, prefix);
    reader->addInput("pT_pl",&pT_pl, prefix);
    reader->addInput("eta_mi",&eta_mi, prefix);
    reader->addInput("eta_pl",&eta_pl, prefix);
    reader->addInput("phi_mi",&phi_mi, prefix);
    reader->addInput("phi_pl",&phi_pl, prefix);
}

//OniaSimpleJet

void OniaSimpleJet::addInputs(TreeReader* reader)
{
    reader->addInput("jt_pt",&jt_pt);
    reader->addInput("jt_rap",&jt_rap);
    reader->addInput("jt_eta",&jt_eta);
    reader->addInput("jt_phi",&jt_phi);
    reader->addInput("jt_pt_JEU_Down",&jt_pt_JEU_Down);
    reader->addInput("jt_pt_JEU_Up",&jt_pt_JEU_Up);
    reader->addInput("jt_pt_jettyCorr",&jt_pt_jettyCorr);
    reader->addInput("z_jettyCorr",&z_jettyCorr);
    reader->addInput("jt_pt_noZJEC",&jt_pt_noZJEC);
    reader->addInput("z", &z);
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

//OniaSimpleRefJet

void OniaSimpleRefJet::addOutputs(TreeWriter* writer)
{
    writer->addOutput("jt_ref_pt",&jt_ref_pt);
    writer->addOutput("jt_ref_rap",&jt_ref_rap);
    writer->addOutput("jt_ref_eta",&jt_ref_eta);
    writer->addOutput("jt_ref_phi",&jt_ref_phi);
    writer->addOutput("gen_z",&gen_z);
    writer->addOutput("jt_pt_genZJEC",&jt_pt_genZJEC);
    writer->addOutput("pthat",&pthat);
}

void OniaSimpleRefJet::addInputs(TreeReader* reader)
{
    reader->addInput("jt_ref_pt",&jt_ref_pt);
    reader->addInput("jt_ref_rap",&jt_ref_rap);
    reader->addInput("jt_ref_eta",&jt_ref_eta);
    reader->addInput("jt_ref_phi",&jt_ref_phi);
    reader->addInput("gen_z",&gen_z);
    reader->addInput("jt_pt_genZJEC",&jt_pt_genZJEC);
}

//OniaWeight
void OniaWeight::addInputs(TreeReader* reader)
{
    reader->addInput("accxeff",&accxeff);
}

//OniaWeight
void OniaWeight::addOutputs(TreeWriter* reader)
{
    reader->addOutput("accxeff",&accxeff);
}