
#include "OniaOutput.h"
#include "TLorentzVector.h"
#include <iostream>


//OniaOutputerQQ

OniaOutputerQQ::OniaOutputerQQ(const char* treeOutName) : OniaOutputer(treeOutName)
{
    addOutput("mass",&mass);
    addOutput("eta",&eta);
    addOutput("phi",&phi);
    addOutput("pT",&pT);
    addOutput("y",&y);
    addOutput("Evt",&Evt);
    addOutput("pT_mi",&pT_mi);
    addOutput("pT_pl",&pT_pl);
    addOutput("eta_mi",&eta_mi);
    addOutput("eta_pl",&eta_pl);
    addOutput("phi_mi",&phi_mi);
    addOutput("phi_pl",&phi_pl);
}

void OniaOutputerQQ::WriteData(const OniaInput& dataIn,Int_t index, Long64_t entry)
{
    TLorentzVector* mom4vec=(TLorentzVector*) dataIn.mom4_RecoQQ->At(index);
    TLorentzVector* mom4vec_mumi = (TLorentzVector*) dataIn.mom4_RecoMu->At(dataIn.mumi_idx[index]);
    TLorentzVector* mom4vec_mupl = (TLorentzVector*) dataIn.mom4_RecoMu->At(dataIn.mupl_idx[index]);
    mass= mom4vec->M();
    pT = mom4vec->Pt();
    y = mom4vec->Rapidity();
    phi = mom4vec->Phi();
    eta = mom4vec->Eta();
    Evt = entry;

    pT_mi = mom4vec_mumi->Pt();
    eta_mi = mom4vec_mumi->Eta();
    phi_mi = mom4vec_mumi->Phi();

    pT_pl = mom4vec_mupl->Pt();
    eta_pl = mom4vec_mupl->Eta();
    phi_pl = mom4vec_mupl->Phi();

    FillEntries();
}