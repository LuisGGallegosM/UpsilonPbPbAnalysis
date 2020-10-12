#include "Main.h"

TTree* oniaSkim(TFile *file,const char* wroteTreeName, std::unique_ptr<Onia_Aux>* auxData);
TTree* jetSkim(TFile *file,const char* wroteTreeName, Onia_Aux* auxData);

/**
 * @brief Execute Onia Upsilon 1S skimming and Jets.
 * 
 * @param filename Name of file where to find the tree to skim.
 * @param outputfilename Name of the root output file to save skimmed data.
 */
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

    TTree* onia_skimmed =  oniaSkim(&file,ONIATTREENAME,&auxData);
    onia_skimmed->Write(0,TObject::kOverwrite);

    TTree* jet_skimmed = jetSkim(&file,JETTTREENAME,auxData.get());
    jet_skimmed->Write(0,TObject::kOverwrite);

    outputfile.Close();
    file.Close();
    std::cout << "Success.\n TTrees wrote to '" << outputfilename<< "' root file\n";
    return;
}

/**
 * @brief Skim 
 * 
 * @param file File where to get the tree to skim.
 * @param wroteTreeName Name of the skimmed tree to write
 * @param auxData A place where to save auxiliary data used for jet skimming.
 * @return Tree with skimmed data.
 */
TTree* oniaSkim(TFile *file,const char* wroteTreeName, std::unique_ptr<Onia_Aux>* auxData)
{
    TTree *myTree = (TTree *)file->Get("hionia/myTree");
    if (myTree == nullptr)
    {
        file->Close();
        std::cout << "tree not found\n";
        return nullptr;
    }

    //execute skim
    OniaCutter cutter;

    OniaSkimmer skimmer = OniaSkimmer(myTree,wroteTreeName);
    TTree* wroteTree = skimmer.Skim(cutter);

    (*auxData) = std::move(skimmer.auxData);

    return wroteTree;
}

/**
 * @brief Same functionality as oniaSkim, but for jets.
 * 
 * @param file File where to get the tree to skim.
 * @param wroteTreeName Name of the skimmed tree to write
 * @param auxData A place where to get auxiliary data used for jet skimming.
 * @return Tree with skimmed data.
 */
TTree* jetSkim(TFile *file,const char* wroteTreeName, Onia_Aux* auxData)
{
    TTree *myTree = (TTree *)file->Get("ak3PFJetAnalyzer/t");
    if (myTree == nullptr)
    {
        file->Close();
        std::cout << "tree not found\n";
        return nullptr;
    }

    JetCutter cutter(auxData);
    
    JetSkimmer skimmer(myTree,wroteTreeName,auxData);
    TTree* wroteTree =skimmer.Skim(cutter);

    return wroteTree;
}