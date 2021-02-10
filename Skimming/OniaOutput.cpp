
#include "OniaOutput.h"
#include "TLorentzVector.h"

//OniaOutputerTree
OniaOutputerTree::OniaOutputerTree(const char* treeOutName)
{
    tree_output= new TTree(treeOutName, "Skimmed tree");
    tree_output->SetMaxTreeSize(MAXTREESIZE);
}

void OniaOutputerTree::addOutput(const char* varName, Float_t* var)
{
    TBranch* branch =tree_output->Branch(varName, var);
    output_branches.push_back(branch);
}

void OniaOutputerTree::addOutput(const char* varName, Int_t* var)
{
    TBranch* branch =tree_output->Branch(varName, var);
    output_branches.push_back(branch);
}

void OniaOutputerTree::Write() 
{
    std::cout << "Total output entries " << tree_output->GetEntries();
    std::cout << " to '" << tree_output->GetName() << "' tree\nDone.\n";
    tree_output->Write(0,TObject::kOverwrite); 
}

//OniaOutputerQQ

OniaOutputerQQ::OniaOutputerQQ(const char* treeOutName) : OniaOutputerTree(treeOutName)
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

void OniaOutputerQQ::WriteData(const Onia_Input& dataIn,Int_t index, Long64_t entry)
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

//OniaOutputerMu

OniaOutputerMu::OniaOutputerMu(const char* treeOutName) : OniaOutputerTree(treeOutName)
{
    addOutput("mass",&mass);
    addOutput("eta",&eta);
    addOutput("phi",&phi);
    addOutput("pT",&pT);
    addOutput("y",&y);
    addOutput("Evt",&Evt);
    addOutput("dR",&dR);

    hist = new TH1F("dR","dR",1000,-0.1,0.1);
}

void OniaOutputerMu::Write()
{
    OniaOutputerTree::Write(); 
    hist->Write(); 
}

void OniaOutputerMu::WriteData(const Onia_Input& dataIn,Int_t index, Long64_t entry)
{
    TLorentzVector* mom4vecRecoMu=(TLorentzVector*) dataIn.mom4_RecoMu->At(index);
    int genMuIndex= dataIn.RecoMuWhichGen[index];
    TLorentzVector* mom4vecGenMu=(TLorentzVector*) dataIn.mom4_GenMu->At(genMuIndex);
    
    mass= mom4vecRecoMu->M();
    pT = mom4vecRecoMu->Pt();
    y = mom4vecRecoMu->Rapidity();
    phi = mom4vecRecoMu->Phi();
    eta = mom4vecRecoMu->Eta();
    Evt = entry;
    float dEta = eta - mom4vecGenMu->Eta();
    float dPhi = phi - mom4vecGenMu->Phi();
    if (dPhi > M_PI) dPhi-=M_PI;
    if (dPhi < -M_PI) dPhi+=M_PI;
    dR=sqrt(dEta*dEta+dPhi*dPhi);

    hist->Fill(dR);
    FillEntries();
}


