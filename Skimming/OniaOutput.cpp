
#include "OniaOutput.h"
#include "TLorentzVector.h"

void OniaOutputerQQ::setOutputs(Skimmer& skimmer)
{
   //output branches
    skimmer.addOutput("mass",&dataOut.mass);
    skimmer.addOutput("eta",&dataOut.eta);
    skimmer.addOutput("phi",&dataOut.phi);
    skimmer.addOutput("pT",&dataOut.pT);
    skimmer.addOutput("y",&dataOut.y);
    skimmer.addOutput("Evt",&dataOut.Evt);
    skimmer.addOutput("pT_mi",&dataOut.pT_mi);
    skimmer.addOutput("pT_pl",&dataOut.pT_pl);
    skimmer.addOutput("eta_mi",&dataOut.eta_mi);
    skimmer.addOutput("eta_pl",&dataOut.eta_pl);
    skimmer.addOutput("phi_mi",&dataOut.phi_mi);
    skimmer.addOutput("phi_pl",&dataOut.phi_pl);

    //auxData.reset(new Onia_Aux());
    //auxData->events.reserve(200000);
}

void OniaOutputerQQ::WriteData(const Onia_Input& dataIn,Int_t index, Long64_t entry)
{
    TLorentzVector* mom4vec=(TLorentzVector*) dataIn.mom4_RecoQQ->At(index);
    TLorentzVector* mom4vec_mumi = (TLorentzVector*) dataIn.mom4_RecoMu->At(dataIn.mumi_idx[index]);
    TLorentzVector* mom4vec_mupl = (TLorentzVector*) dataIn.mom4_RecoMu->At(dataIn.mupl_idx[index]);
    dataOut.mass= mom4vec->M();
    dataOut.pT = mom4vec->Pt();
    dataOut.y = mom4vec->Rapidity();
    dataOut.phi = mom4vec->Phi();
    dataOut.eta = mom4vec->Eta();
    dataOut.Evt = entry;

    dataOut.pT_mi = mom4vec_mumi->Pt();
    dataOut.eta_mi = mom4vec_mumi->Eta();
    dataOut.phi_mi = mom4vec_mumi->Phi();

    dataOut.pT_pl = mom4vec_mupl->Pt();
    dataOut.eta_pl = mom4vec_mupl->Eta();
    dataOut.phi_pl = mom4vec_mupl->Phi();

    //auxData->events.insert({entry,dataOut});
}

//mu

void OniaOutputerMu::setOutputs(Skimmer& skimmer)
{
   //output branches
    skimmer.addOutput("mass",&dataOut.mass);
    skimmer.addOutput("eta",&dataOut.eta);
    skimmer.addOutput("phi",&dataOut.phi);
    skimmer.addOutput("pT",&dataOut.pT);
    skimmer.addOutput("y",&dataOut.y);
    skimmer.addOutput("Evt",&dataOut.Evt);
    skimmer.addOutput("dR",&dataOut.dR);

    //auxData.reset(new Onia_Aux());
    //auxData->events.reserve(200000);
}

void OniaOutputerMu::WriteData(const Onia_Input& dataIn,Int_t index, Long64_t entry)
{
    TLorentzVector* mom4vecRecoMu=(TLorentzVector*) dataIn.mom4_RecoMu->At(index);
    int genMuIndex= dataIn.RecoMuWhichGen[index];
    TLorentzVector* mom4vecGenMu=(TLorentzVector*) dataIn.mom4_GenMu->At(genMuIndex);
    dataOut.mass= mom4vecRecoMu->M();
    dataOut.pT = mom4vecRecoMu->Pt();
    dataOut.y = mom4vecRecoMu->Rapidity();
    dataOut.phi = mom4vecRecoMu->Phi();
    dataOut.eta = mom4vecRecoMu->Eta();
    dataOut.Evt = entry;
    float dEta = dataOut.eta - mom4vecGenMu->Eta();
    float dPhi = dataOut.phi - mom4vecGenMu->Phi();
    if (dPhi > M_PI) dPhi-=M_PI;
    if (dPhi < -M_PI) dPhi+=M_PI;
    dataOut.dR=sqrt(dEta*dEta+dPhi*dPhi);
}