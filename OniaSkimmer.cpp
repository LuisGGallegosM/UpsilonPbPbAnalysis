

#include "OniaSkimmer.h"

/**
 * @brief Construct a new OniaSkimmer:: OniaSkimmer object
 * 
 * @param myTree Pointer to the tree to skim from.
 */
OniaSkimmer::OniaSkimmer(TTree* myTree) : 
input_branches(),oniaDataIn(), tree(myTree)
{
    TString name(tree->GetName());
    tree_output= new TTree(name + "_Skimmed", "Skimmed tree");
    tree_output->SetMaxTreeSize(MAXTREESIZE);
    InitBranches();
}

/**
 * @brief Execute the skimming
 * 
 */
void OniaSkimmer::Skim(std::function<bool(Onia_Input*,long)> cutter)
{
    int block =0;
    Long64_t entries= tree->GetEntries();

    printf("Onia Skimming start...");
    printf("%lld entries from '%s' tree\n", entries, tree->GetName());

    for(Long64_t i=0;i<entries;++i)
    {
        if ((i % (entries/20)) ==0)
        {
            printf("Processing: %i%%\n",block*5);
            ++block;
        }
            
        GetEntries(i);

        for(Long64_t j=0;j<oniaDataIn.size;++j)
        {
            if (cutter(&oniaDataIn,j))
            {
                WriteData(j);
                tree_output->Fill();
            }
        }

    }
    printf("Total readed entries %lld from '%s' tree\n", entries, tree->GetName());
    printf("Total output entries %lld to '%s' tree\n", tree_output->GetEntries(),tree_output->GetName());
    printf("..Saving tree\n");
    return;
}

void OniaSkimmer::WriteData(long index)
{
    TLorentzVector* mom4vec=(TLorentzVector*) oniaDataIn.mom4->At(index);
    oniaDataOut.mass= mom4vec->M();
    oniaDataOut.pT = mom4vec->Pt();
    oniaDataOut.y = mom4vec->Rapidity();
    oniaDataOut.phi = mom4vec->Phi();
    oniaDataOut.eta = mom4vec->Eta();
}

/**
 * @brief initializes input and output branches for writing and reading
 * by using functions fill and GetEntries
 */
void OniaSkimmer::InitBranches()
{
    TBranch* branch;

    //input branches
    branch = tree->GetBranch("Reco_QQ_4mom");
    branch->SetAddress(&oniaDataIn.mom4);
    input_branches.push_back(branch);

    branch = tree->GetBranch("Reco_QQ_size");
    branch->SetAddress(&oniaDataIn.size);
    input_branches.push_back(branch);

    branch = tree->GetBranch("Reco_QQ_mupl_idx");
    branch->SetAddress(oniaDataIn.mupl_idx);
    input_branches.push_back(branch);

    branch = tree->GetBranch("Reco_QQ_mumi_idx");
    branch->SetAddress(oniaDataIn.mumi_idx);
    input_branches.push_back(branch);

    branch = tree->GetBranch("Reco_mu_SelectionType");
    branch->SetAddress(oniaDataIn.SelectionType);
    input_branches.push_back(branch);

    branch = tree->GetBranch("Reco_mu_nTrkWMea");
    branch->SetAddress(oniaDataIn.nTrkWMea);
    input_branches.push_back(branch);

    branch = tree->GetBranch("Reco_mu_nPixWMea");
    branch->SetAddress(oniaDataIn.nPixWMea);
    input_branches.push_back(branch);

    branch = tree->GetBranch("Reco_mu_dxy");
    branch->SetAddress(oniaDataIn.dxy);
    input_branches.push_back(branch);

    branch = tree->GetBranch("Reco_mu_dz");
    branch->SetAddress(oniaDataIn.dz);
    input_branches.push_back(branch);

    branch = tree->GetBranch("Reco_QQ_VtxProb");
    branch->SetAddress(oniaDataIn.VtxProb);
    input_branches.push_back(branch);

    branch = tree->GetBranch("Reco_QQ_trig");
    branch->SetAddress(oniaDataIn.trig);
    input_branches.push_back(branch);

    //output branches
    tree_output->Branch("mass", &oniaDataOut.mass,"mass/F");
    tree_output->Branch("eta", &oniaDataOut.eta,"eta/F");
    tree_output->Branch("phi", &oniaDataOut.phi,"phi/F");
    tree_output->Branch("pT", &oniaDataOut.pT,"pT/F");
    tree_output->Branch("y", &oniaDataOut.y,"y/F");

    return;
}

/**
 * @brief call function GetEntry for all readed branches.
 * 
 * @param index the event index for GetEntry of each branch
 */
void OniaSkimmer::GetEntries(long index)
{
    for(int i=0 ;i<input_branches.size();i++)
    {
        input_branches[i]->GetEntry(index);
    }
    return;
}

TTree* OniaSkimmer::GetTree()
{
    return tree_output;
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