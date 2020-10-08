#include <iostream>
#include <vector>
#include "TTree.h"
#include "TFile.h"
#include "TString.h"

#include "OniaSkimmer.h"
#include "OniaCutter.h"
#include "JetSkimmer.h"
#include "JetCutter.h"

#if defined(__CLING__)
#include "OniaSkimmer.cpp"
#include "OniaCutter.cpp"
#include "JetSkimmer.cpp"
#include "JetCutter.cpp"
#endif

bool oniaSkim(TFile *file);
bool jetSkim(TFile *file);

void Main()
{
    //input file
    TString filename("files/merged_HiForestAOD.root");
    TFile file(filename.Data(), "READ");

    //ooutput file
    TString outputfilename("files/merged_HiForestAOD_skimmed.root");
    TFile outputfile(outputfilename.Data(), "RECREATE");

    if (file.IsZombie()) //input file is found?
    {
        std::cout << "file cannot readed\n";
        return;
    }

    oniaSkim(&file);
    jetSkim(&file);

    outputfile.Close();
    file.Close();
    std::cout << "Success.\n TTrees wrote to '" << outputfilename.Data() << "' root file\n";
    return;
}

bool oniaSkim(TFile *file)
{
    TTree *myTree = (TTree *)file->Get("hionia/myTree");
    if (myTree == nullptr)
    {
        file->Close();
        std::cout << "tree not found\n";
        return false;
    }

    //execute skim
    OniaCutter cutter;

    OniaSkimmer skimmer = OniaSkimmer(myTree);
    skimmer.Skim(cutter);

    TTree *outputTree = skimmer.GetTree();

    outputTree->Write(0,TObject::kOverwrite);
    return true;
}

bool jetSkim(TFile *file)
{
    TTree *myTree = (TTree *)file->Get("ak3PFJetAnalyzer/t");
    if (myTree == nullptr)
    {
        file->Close();
        std::cout << "tree not found\n";
        return false;
    }

    //execute skim
    JetCutter cutter;
    
    JetSkimmer skimmer = JetSkimmer(myTree);
    skimmer.Skim(cutter);

    TTree *outputTree = skimmer.GetTree();

    outputTree->Write(0,TObject::kOverwrite);
    return true;
}



#if !defined(__CLING__)

int main(int argc, char **argv)
{
    Main();
    return 0;
}

#endif