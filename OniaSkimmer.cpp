

#include "OniaSkimmer.h"

OniaSkimmer::OniaSkimmer(TTree* myTree) : Skimmer(myTree) 
{
    TBranch* branch;

    //input branches
    branch = tree->GetBranch("Reco_QQ_4mom");
    branch->SetAddress(&dataIn.mom4);
    input_branches.push_back(branch);

    branch = tree->GetBranch("Reco_QQ_size");
    branch->SetAddress(&dataIn.size);
    input_branches.push_back(branch);

    branch = tree->GetBranch("Reco_QQ_mupl_idx");
    branch->SetAddress(dataIn.mupl_idx);
    input_branches.push_back(branch);

    branch = tree->GetBranch("Reco_QQ_mumi_idx");
    branch->SetAddress(dataIn.mumi_idx);
    input_branches.push_back(branch);

    branch = tree->GetBranch("Reco_mu_SelectionType");
    branch->SetAddress(dataIn.SelectionType);
    input_branches.push_back(branch);

    branch = tree->GetBranch("Reco_mu_nTrkWMea");
    branch->SetAddress(dataIn.nTrkWMea);
    input_branches.push_back(branch);

    branch = tree->GetBranch("Reco_mu_nPixWMea");
    branch->SetAddress(dataIn.nPixWMea);
    input_branches.push_back(branch);

    branch = tree->GetBranch("Reco_mu_dxy");
    branch->SetAddress(dataIn.dxy);
    input_branches.push_back(branch);

    branch = tree->GetBranch("Reco_mu_dz");
    branch->SetAddress(dataIn.dz);
    input_branches.push_back(branch);

    branch = tree->GetBranch("Reco_QQ_VtxProb");
    branch->SetAddress(dataIn.VtxProb);
    input_branches.push_back(branch);

    branch = tree->GetBranch("Reco_QQ_trig");
    branch->SetAddress(dataIn.trig);
    input_branches.push_back(branch);

    //output branches
    tree_output->Branch("mass", &dataOut.mass,"mass/F");
    tree_output->Branch("eta", &dataOut.eta,"eta/F");
    tree_output->Branch("phi", &dataOut.phi,"phi/F");
    tree_output->Branch("pT", &dataOut.pT,"pT/F");
    tree_output->Branch("y", &dataOut.y,"y/F");

    return;
}

void OniaSkimmer::WriteData(long index)
{
    TLorentzVector* mom4vec=(TLorentzVector*) dataIn.mom4->At(index);
    dataOut.mass= mom4vec->M();
    dataOut.pT = mom4vec->Pt();
    dataOut.y = mom4vec->Rapidity();
    dataOut.phi = mom4vec->Phi();
    dataOut.eta = mom4vec->Eta();
}

//***********************************************
//Onia structs

Onia_Input::Onia_Input()
{
    mom4 = new TClonesArray("TLorentzVector");
    mupl_idx = new Int_t[maxBranchSize];
    mumi_idx = new Int_t[maxBranchSize];
    SelectionType = new Int_t[maxBranchSize];
    nTrkWMea = new Int_t[maxBranchSize];
    nPixWMea = new Int_t[maxBranchSize];
    dxy = new Float_t[maxBranchSize];
    dz = new Float_t[maxBranchSize]; 
    VtxProb = new Float_t[maxBranchSize];
    trig = new ULong64_t[maxBranchSize];
}

Onia_Input::~Onia_Input()
{
    delete mom4;
    delete[] mupl_idx;
    delete[] mumi_idx;
    delete[] SelectionType;
    delete[] nTrkWMea;
    delete[] nPixWMea;
    delete[] dxy;
    delete[] dz;
    delete[] VtxProb;
    delete[] trig;
}