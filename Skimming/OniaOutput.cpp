
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

OniaOutputerQQ::OniaOutputerQQ(const char* treeOutName) : OniaOutputerTree(treeOutName) , dataOut()
{
   //output branches
    addOutput("mass",&dataOut.mass);
    addOutput("eta",&dataOut.eta);
    addOutput("phi",&dataOut.phi);
    addOutput("pT",&dataOut.pT);
    addOutput("y",&dataOut.y);
    addOutput("Evt",&dataOut.Evt);
    addOutput("pT_mi",&dataOut.pT_mi);
    addOutput("pT_pl",&dataOut.pT_pl);
    addOutput("eta_mi",&dataOut.eta_mi);
    addOutput("eta_pl",&dataOut.eta_pl);
    addOutput("phi_mi",&dataOut.phi_mi);
    addOutput("phi_pl",&dataOut.phi_pl);

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

    FillEntries();
}

//OniaOutputerMu

OniaOutputerMu::OniaOutputerMu(const char* treeOutName) : OniaOutputerTree(treeOutName) , dataOut()
{
   //output branches
    addOutput("mass",&dataOut.mass);
    addOutput("eta",&dataOut.eta);
    addOutput("phi",&dataOut.phi);
    addOutput("pT",&dataOut.pT);
    addOutput("y",&dataOut.y);
    addOutput("Evt",&dataOut.Evt);
    addOutput("dR",&dataOut.dR);

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

    hist->Fill(dataOut.dR);
    FillEntries();
}


