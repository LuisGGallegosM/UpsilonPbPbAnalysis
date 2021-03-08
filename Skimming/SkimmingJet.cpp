
#include"SkimmingJet.h"

TTree* GetTree(TFile* file,const std::string& name);
using std::ofstream;

/**
 * @brief Execute Upsilon 1S skimming: Onia and Jets.
 * 
 * @param filename Name of file where to find the tree to skim.
 * @param outputfilename Name of the root output file to save skimmed data.
 * @param cut Cut parameters
 */
void SkimmingJet(const char* filename,const char* outputfilename, const char* configname)
{
    std::cout << "\nSKIMMING\n";
    std::cout << "Reading input file: " << filename <<'\n';
    std::cout << "Writing to output file: " << outputfilename <<'\n';
    std::cout << "Cut configuration file: " << configname <<'\n';

    //input file
    TFile* file = OpenFile(filename,"READ");

    //output file
    TFile* outputfile = OpenFile(outputfilename, "CREATE");

    CutParams cut;
    cut.deserialize(configname);

    if(!cut.isValid())
    {
        std::cerr << "Error: Invalid cut parameters.\n";
        return;
    }

    //copy cut config file
    CopyFile(configname,ReplaceExtension(outputfilename,".cutconf").data());

    //skim the data
    TTree *oniaTree = GetTree(file,"hionia/myTree");
    TTree *jetTree = GetTree(file,"ak4PFJetAnalyzer/t");
    TTree *skimTree = GetTree(file,"skimanalysis/HltTree");

    oniaTree->AddFriend(jetTree);
    skimTree->AddFriend(skimTree);

    //execute skim
    std::unique_ptr<Skimmer> skimmer = createSkimmer(oniaTree,&cut,ONIATTREENAME);
    skimmer->Skim();
    skimmer->Write();

    outputfile->Close();
    file->Close();
    delete file;
    std::cout << "Success.\n TTrees wrote to '" << outputfilename<< "' root file\n";
    return;
}

TTree* GetTree(TFile* file,const std::string& name)
{
    TTree* tree =(TTree *)file->Get(name.data());
    if (tree == nullptr)
    {
        throw std::invalid_argument(name + " tree not found\n");
    }
    return tree;
}