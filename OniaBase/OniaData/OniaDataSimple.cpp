
#include "OniaDataSimple.h"

//OniaSimpleInfo

void OniaSimpleInfo::addOutputs(TreeWriter* writer)
{
    writer->addOutput("Evt",&Evt);
    writer->addOutput("pdfId",&pdgId);
}

//OniaSimpleQQ

void OniaSimpleQQ::addOutputs(TreeWriter* writer)
{
    writer->addOutput("mass",&mass);
    writer->addOutput("eta",&eta);
    writer->addOutput("phi",&phi);
    writer->addOutput("pT",&pT);
    writer->addOutput("y",&y);
}

void OniaSimpleQQ::Write(TLorentzVector* dimuon)
{
    mass= dimuon->M();
    pT = dimuon->Pt();
    y = dimuon->Rapidity();
    phi = dimuon->Phi();
    eta = dimuon->Eta();
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

void OniaSimpleMu::addOutputs(TreeWriter* writer)
{
    writer->addOutput("pT_mi",&pT_mi);
    writer->addOutput("pT_pl",&pT_pl);
    writer->addOutput("eta_mi",&eta_mi);
    writer->addOutput("eta_pl",&eta_pl);
    writer->addOutput("phi_mi",&phi_mi);
    writer->addOutput("phi_pl",&phi_pl);
}