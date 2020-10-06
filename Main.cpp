#include <iostream>
#include <vector>
#include "TTree.h"
#include "TFile.h"
#include "TString.h"

#include "OniaSkimmer.h"
#include "OniaCutter.h"

#if defined(__CLING__)
    #include "OniaSkimmer.cpp"
    #include "OniaCutter.cpp"
#endif

void Main()
{
    //input file
    TString filename("files/merged_HiForestAOD.root");
    TFile file(filename.Data(),"READ");

    //ooutput file
    TString outputfilename("files/merged_HiForestAOD_skimmed.root");
    TFile outputfile(outputfilename.Data(),"RECREATE");

    if(file.IsZombie())//input file is found?
    {
        std::cout << "file cannot readed\n";
        return;
    }

    TTree* myTree = (TTree*)file.Get("hionia/myTree");
    if (myTree==nullptr)
    {
        file.Close();
        std::cout << "tree not found\n";
        return;
    }

    //execute skim
    std::function<bool(Onia_Input*,long)> cutter = OniaCutter();

    OniaSkimmer skimmer = OniaSkimmer(myTree);
    skimmer.Skim(cutter);

    TTree* outputTree= skimmer.GetTree();

    outputTree->Write();

    outputfile.Close();
    file.Close();
    std::cout << "Success\n";
    return;
}

#if !defined(__CLING__)

int main(int argc, char** argv)
{
    Main();
    return 0;
}

#endif