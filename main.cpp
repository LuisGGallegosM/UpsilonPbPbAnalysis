
#include <iostream>
#include "TTree.h"
#include "TFile.h"
#include "TString.h"
#include "Skimmer.h"

int main()
{
    TString filename("files/merged_HiForestAOD.root");
    TFile file(filename.Data(),"READ");

    TString outputfilename("files/merged_HiForestAOD_skimmed.root");
    TFile outputfile(outputfilename.Data(),"RECREATE");

    if(file.IsZombie())
    {
        std::cout << "file cannot readed\n";
        return 0;
    }

    Skimmer skimmer = Skimmer();

    TTree* myTree = (TTree*)file.Get("hionia/myTree");
    if (myTree==nullptr)
    {
        file.Close();
        std::cout << "tree not found\n";
        return 0;
    }

    TTree* skimmedTree= skimmer.Skim(myTree);

    skimmedTree->Write();

    outputfile.Close();
    file.Close();
    std::cout << "Success\n";
    return 0;
}