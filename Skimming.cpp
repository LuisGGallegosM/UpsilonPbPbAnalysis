#include "Main.h"

bool oniaSkim(TFile *file,const char* wroteTreeName, std::unique_ptr<Onia_Aux>* auxData);
bool jetSkim(TFile *file,const char* wroteTreeName, Onia_Aux* auxData);

void Skim(const char* filename,const char* outputfilename)
{
    //input file
    TFile file(filename, "READ");

    //output file
    TFile outputfile(outputfilename, "RECREATE");

    if (file.IsZombie()) //input file is found?
    {
        std::cout << "file cannot readed\n";
        return;
    }

    //tree to write skimmed data
    std::unique_ptr<Onia_Aux> auxData;

    oniaSkim(&file,oniaTTreeName,&auxData);
    jetSkim(&file,jetTTreeName,auxData.get());

    outputfile.Close();
    file.Close();
    std::cout << "Success.\n TTrees wrote to '" << outputfilename<< "' root file\n";
    return;
}

bool oniaSkim(TFile *file,const char* wroteTreeName, std::unique_ptr<Onia_Aux>* auxData)
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

    OniaSkimmer skimmer = OniaSkimmer(myTree,wroteTreeName);
    TTree* wroteTree = skimmer.Skim(cutter);

    (*auxData) = std::move(skimmer.auxData);

    wroteTree->Write(0,TObject::kOverwrite);

    return true;
}

bool jetSkim(TFile *file,const char* wroteTreeName, Onia_Aux* auxData)
{
    TTree *myTree = (TTree *)file->Get("ak3PFJetAnalyzer/t");
    if (myTree == nullptr)
    {
        file->Close();
        std::cout << "tree not found\n";
        return false;
    }

    //execute skim
    JetCutter cutter(auxData);
    
    JetSkimmer skimmer(myTree,wroteTreeName,auxData);
    TTree* wroteTree =skimmer.Skim(cutter);

    wroteTree->Write(0,TObject::kOverwrite);
    return true;
}