
#include "Skimming.h"

/**
 * @brief Execute Upsilon 1S skimming: Onia and Jets.
 * 
 * @param filename Name of file where to find the tree to skim.
 * @param outputfilename Name of the root output file to save skimmed data.
 * @param cut Cut parameters
 */
void SkimmingOnia(const char* filename,const char* outputfilename, const char* configname)
{
    std::cout << "\nSKIMMING ONIA\n";
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

    //get tree to skim
    TTree *myTree = GetTree(file,"hionia/myTree");

    //execute skim
    std::unique_ptr<Skimmer> skimmer = createOniaSkimmer(myTree,&cut,ONIATTREENAME);
    skimmer->Skim();

    if (skimmer==nullptr) return;
    skimmer->Write();

    outputfile->Close();
    file->Close();
    delete file;
    std::cout << "Success.\n TTrees wrote to '" << outputfilename<< "' root file\n";
    return;
}


/**
 * @brief Execute Upsilon 1S skimming: Onia and Jets.
 * 
 * @param filename Name of file where to find the tree to skim.
 * @param outputfilename Name of the root output file to save skimmed data.
 * @param cut Cut parameters
 */
void SkimmingOniaJet(const char* filename,const char* outputfilename, const char* configname)
{
    std::cout << "\nSKIMMING ONIA AND JETS\n";
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
    oniaTree->AddFriend(skimTree);

    //execute skim
    std::unique_ptr<Skimmer> skimmer = createJetSkimmer(oniaTree,&cut,ONIATTREENAME);
    skimmer->Skim();
    skimmer->Write();

    outputfile->Close();
    file->Close();
    delete file;
    std::cout << "Success.\n TTrees wrote to '" << outputfilename<< "' root file\n";
    return;
}

#if !defined(__CLING__)

int main(int argc, char **argv)
{
    if (argc == 4)
        SkimmingOnia(argv[1],argv[2],argv[3]);
    else if (argc ==5)
    {
        std::string flag= argv[1];
        if(flag=="-jet")
        {
            SkimmingOniaJet(argv[2],argv[3],argv[4]);
        }
        else
        {
            std::cerr << "Incorrect option flag\n";  
        }
    }
    else
    {
          std::cerr << "Incorrect number of parameters\n";  
    }
    return 0;
}

#endif