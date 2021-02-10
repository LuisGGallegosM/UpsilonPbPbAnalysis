#include "Skimming.h"

std::unique_ptr<OniaOutputer> oniaSkim(TFile *file,const char* wroteTreeName, const cutParams* kineCut);
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
    std::unique_ptr<OniaOutputer> onia_skimmed =  oniaSkim(file,ONIATTREENAME,&cut);
    if (onia_skimmed==nullptr) return;
    onia_skimmed->Write();

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
std::unique_ptr<OniaOutputer> oniaSkim(TFile *file,const char* wroteTreeName, const cutParams* cut)
{
    TTree *myTree = (TTree *)file->Get("hionia/myTree");
    if (myTree == nullptr)
    {
        std::cout << "hionia/myTree tree not found\n";
        return nullptr;
    }

    //execute skim
    //std::unique_ptr<OniaCutter> cutter(new OniaCutterRecoQQ(cut));
    //std::unique_ptr<OniaOutputer> outputer(new OniaOutputerQQ());
    std::unique_ptr<OniaCutter> cutter(new OniaCutterRecoMu());
    std::unique_ptr<OniaOutputer> outputer(new OniaOutputerMu(wroteTreeName));

    OniaSkimmer skimmer = OniaSkimmer(myTree,outputer.get(),cutter.get());
    skimmer.Skim();

    return outputer;
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