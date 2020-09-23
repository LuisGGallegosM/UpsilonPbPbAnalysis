
#include "TTree.h"
#include "TLorentzVector.h"
#include "TString.h"
#include "TClonesArray.h"
#include "Skimmer.h"

Skimmer::Skimmer()
{
    toKeep= TString("Reco_QQ_4mom");
}

TTree* Skimmer::Skim(TTree* tree)
{
    TClonesArray* Reco_QQ_4mom= new TClonesArray("TLorentzVector");

    //Create new Tree
    TString newTreeName(tree->GetName());
    newTreeName.Append("_Skimmed");
    TTree* skimmedTree = new TTree(newTreeName.Data(),"Skimmed Tree");
    skimmedTree->Branch(toKeep.Data(),"TClonesArray",&Reco_QQ_4mom,32000,0);

    TBranch* Reco_QQ_4momBranch = skimmedTree->GetBranch(toKeep.Data());
    TBranch* mom4branch = tree->GetBranch(toKeep.Data());
    mom4branch->Print();

    Reco_QQ_4momBranch->SetAddress(&Reco_QQ_4mom);
    mom4branch->SetAddress(&Reco_QQ_4mom);

    for(int i=0;i<1000;i++)
    {
        mom4branch->GetEntry(i);
        skimmedTree->Fill();
    }
    mom4branch->Print();
    
    delete Reco_QQ_4mom;
    return skimmedTree;
}