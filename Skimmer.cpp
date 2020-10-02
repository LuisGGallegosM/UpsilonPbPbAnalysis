
#include "TTree.h"
#include "TLorentzVector.h"
#include "TString.h"
#include "TClonesArray.h"
#include "RooDataSet.h"
#include "RooRealVar.h"
#include "RooPlot.h"
#include "new"

#include "Skimmer.h"

using namespace RooFit;

/**
 * @brief Construct a new Skimmer:: Skimmer object
 * 
 * @param myTree Pointer to the tree to skim from.
 */
Skimmer::Skimmer(TTree* myTree) : 
input_branches(),diMuonData(), tree(myTree)
{
    TString name(tree->GetName());
    tree_output= new TTree(name + "_Skimmed", "Skimmed tree");
    InitBranches();
}

/**
 * @brief Execute the skimming
 * 
 */
void Skimmer::Skim()
{
    int block =0;
    int entries= tree->GetEntries()/10;

    for(int i=0;i<entries;++i)
    {
        if ((i % entries/20) ==0)
        {
            printf("Processing: %i%%\n",block*5);
            ++block;
        }
            
        GetEntries(i);

        for(int j=0;j<diMuonData.size;++j)
        {
            //write data to output
            TLorentzVector* mom4vec=(TLorentzVector*) diMuonData.mom4->At(j);
            diMuonDataOut.mass= mom4vec->M();
            diMuonDataOut.pT = mom4vec->Pt();
            diMuonDataOut.y = mom4vec->Rapidity();
            diMuonDataOut.phi = mom4vec->Phi();
            diMuonDataOut.eta = mom4vec->Eta();

            tree_output->Fill();
        }

    }
    printf("Processing: 100%%\n");
    printf("Total readed entries %i from %s tree\n", entries, tree->GetName());
    printf("Total output entries %lld to %s tree\n", tree_output->GetEntries(),tree_output->GetName());
    printf("..Saving tree\n");
    return;
}

/**
 * @brief initializes input and output branches for writing and reading
 * by using functions fill and GetEntries
 */
void Skimmer::InitBranches()
{
    TBranch* branch;

    input_branches.clear();

    //input branches
    branch = tree->GetBranch("Reco_QQ_4mom");
    branch->SetAddress(&diMuonData.mom4);
    input_branches.push_back(branch);

    branch = tree->GetBranch("Reco_QQ_size");
    branch->SetAddress(&diMuonData.size);
    input_branches.push_back(branch);

    branch = tree->GetBranch("Reco_QQ_mupl_idx");
    branch->SetAddress(diMuonData.Reco_QQ_mupl_idx);
    input_branches.push_back(branch);

    branch = tree->GetBranch("Reco_QQ_mumi_idx");
    branch->SetAddress(diMuonData.Reco_QQ_mumi_idx);
    input_branches.push_back(branch);

    //output branches
    branch = tree_output->Branch("mass", &diMuonDataOut.mass,"mass/F");
    branch = tree_output->Branch("eta", &diMuonDataOut.eta,"eta/F");
    branch = tree_output->Branch("phi", &diMuonDataOut.phi,"phi/F");
    branch = tree_output->Branch("pT", &diMuonDataOut.pT,"pT/F");
    branch = tree_output->Branch("y", &diMuonDataOut.y,"y/F");

    return;
}

/**
 * @brief call function GetEntry for all readed branches.
 * 
 * @param index the event index for GetEntry of each branch
 */
void Skimmer::GetEntries(int index)
{
    for(int i=0 ;i<input_branches.size();i++)
    {
        input_branches[i]->GetEntry(index);
    }
    return;
}

TTree* Skimmer::GetTree()
{
    return tree_output;
}

//***********************************************
//Muon

Muon_Input::Muon_Input()
{
    mom4 = new TClonesArray("TLorentzVector");
    Reco_QQ_mupl_idx = new Int_t[maxBranchSize];
    Reco_QQ_mumi_idx = new Int_t[maxBranchSize];
}

Muon_Input::~Muon_Input()
{
    delete mom4;
    delete[] Reco_QQ_mupl_idx;
    delete[] Reco_QQ_mumi_idx;
}