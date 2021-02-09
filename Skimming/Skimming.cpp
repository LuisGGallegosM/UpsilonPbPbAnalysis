#include "Skimming.h"

TTree* oniaSkim(TFile *file,const char* wroteTreeName, std::unique_ptr<Onia_Aux>* auxData, const cutParams* kineCut);
TTree* jetSkim(TFile *file,const char* wroteTreeName, Onia_Aux* auxData);
void SetCutParams(cutParams* kineCut);

using std::ofstream;

/**
 * @brief Execute Upsilon 1S skimming: Onia and Jets.
 * 
 * @param filename Name of file where to find the tree to skim.
 * @param outputfilename Name of the root output file to save skimmed data.
 * @param cut Cut parameters
 */
void Skimming(const char* filename,const char* outputfilename, const char* configname)
{
    std::cout << "\nSKIMMING\n";
    std::cout << "Reading input file: " << filename <<'\n';
    std::cout << "Writing to output file: " << outputfilename <<'\n';
    std::cout << "Cut configuration file: " << configname <<'\n';

    //input file
    TFile* file = TFile::Open(filename, "READ");

    //input file is found? is output filename valid?
    if (file==nullptr) 
    {
        std::cerr << "file "<< filename <<" cannot be read\n";
        return;
    }

    //output file
    TFile outputfile(outputfilename, "CREATE");

    if (outputfile.IsZombie())
    {
        std::cerr << "file "<< outputfilename <<" cannot be wrote or already exists\n";
        return;
    }

    cutParams cut;
    cut.deserialize(configname);

    if(!cut.isValid())
    {
        std::cerr << "Error: Invalid cut parameters.\n";
        return;
    }

    //copy cut config file
    CopyFile(configname,ReplaceExtension(outputfilename,".cutconf").data());

    //tree to write skimmed data
    std::unique_ptr<Onia_Aux> auxData;

    TTree* onia_skimmed =  oniaSkim(file,ONIATTREENAME,&auxData,&cut);
    if (onia_skimmed==nullptr) return;
    onia_skimmed->Write(0,TObject::kOverwrite);

    //TTree* jet_skimmed = jetSkim(file,JETTTREENAME,auxData.get());
    //if (jet_skimmed==nullptr) return;
    //jet_skimmed->Write(0,TObject::kOverwrite);

    outputfile.Close();
    file->Close();
    delete file;
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
TTree* oniaSkim(TFile *file,const char* wroteTreeName, std::unique_ptr<Onia_Aux>* auxData, const cutParams* cut)
{
    TTree *myTree = (TTree *)file->Get("hionia/myTree");
    if (myTree == nullptr)
    {
        std::cout << "hionia/myTree tree not found\n";
        return nullptr;
    }

    //execute skim
    OniaCutter cutter(cut);

    OniaSkimmer skimmer = OniaSkimmer(myTree,wroteTreeName,&cutter);
    TTree* wroteTree = skimmer.Skim();

    (*auxData) = std::move(skimmer.auxData);

    return wroteTree;
}

#if !defined(__CLING__)

int main(int argc, char **argv)
{
    if (argc == 4)
        Skimming(argv[1],argv[2],argv[3]);
    else
    {
          std::cerr << "Incorrect number of parameters\n";  
    }
    return 0;
}

#endif