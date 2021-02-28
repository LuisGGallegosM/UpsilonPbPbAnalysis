#include "Skimming.h"

std::unique_ptr<OniaSkimmer> oniaSkim(TFile *file,const char* wroteTreeName, const cutParams* kineCut);
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
    TFile* file = OpenFile(filename,"READ");

    //output file
    TFile* outputfile = OpenFile(outputfilename, "CREATE");

    cutParams cut;
    cut.deserialize(configname);

    if(!cut.isValid())
    {
        std::cerr << "Error: Invalid cut parameters.\n";
        return;
    }

    //copy cut config file
    CopyFile(configname,ReplaceExtension(outputfilename,".cutconf").data());

    //skim the data
    std::unique_ptr<OniaSkimmer> skimmer = oniaSkim(file,ONIATTREENAME,&cut);
    if (skimmer==nullptr) return;
    skimmer->Write();

    outputfile->Close();
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
 * @return Output with skimmed data.
 */
std::unique_ptr<OniaSkimmer> oniaSkim(TFile *file,const char* wroteTreeName, const cutParams* cut)
{
    TTree *myTree = (TTree *)file->Get("hionia/myTree");
    if (myTree == nullptr)
    {
        std::cout << "hionia/myTree tree not found\n";
        return nullptr;
    }

    //execute skim
    OniaReader* reader =new OniaReader(myTree,cut->getIsMC());
    OniaCutter* cutter =new OniaCutterRecoQQ(cut);
    OniaWriter* writer =new OniaWriterFull(wroteTreeName,QQtype::Reco);

    std::unique_ptr<OniaSkimmer> skimmer (new OniaSkimmer(reader,cutter, writer));
    skimmer->Skim();

    return skimmer;
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