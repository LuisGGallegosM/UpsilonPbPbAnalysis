
#include "AccEff.h"

std::unique_ptr<AccOutputer> AccEff(TFile *file,const char* wroteTreeName, const cutParams* kineCut);
void SetCutParams(cutParams* kineCut);

using std::ofstream;

/**
 * @brief Execute Upsilon 1S skimming: Onia and Jets.
 * 
 * @param filename Name of file where to find the tree to skim.
 * @param outputfilename Name of the root output file to save skimmed data.
 * @param cut Cut parameters
 */
void AccEff(const char* filename,const char* outputfilename, const char* configname)
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

    //tree to write skimmed data
    std::unique_ptr<AccOutputer> onia_skimmed =  AccTest(file,ONIATTREENAME,&cut);
    if (onia_skimmed==nullptr) return;
    onia_skimmed->Write();

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
 * @param auxData A place where to save auxiliary data used for jet skimming.
 * @return Tree with skimmed data.
 */
std::unique_ptr<AccOutputer> AccTest(TFile *file,const char* wroteTreeName, const cutParams* cut)
{
    TTree *myTree = (TTree *)file->Get("hionia/myTree");
    if (myTree == nullptr)
    {
        std::cout << "hionia/myTree tree not found\n";
        return nullptr;
    }

    std::unique_ptr<AccEffCutter> cutter(new AccEffCutter(cut));
    std::unique_ptr<AccOutputer> outputer(new AccOutputer(wroteTreeName));

    AccTester accTester(myTree,outputer.get(),cutter.get());
    accTester.Test();

    return outputer;
}

#if !defined(__CLING__)

int main(int argc, char **argv)
{
    if (argc == 4)
        AccEff(argv[1],argv[2],argv[3]);
    else
    {
          std::cerr << "Incorrect number of parameters\n";  
    }
    return 0;
}

#endif