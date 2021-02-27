
#include "AccEff.h"

std::unique_ptr<AccEffAnalyzer> AccEffTest(TFile *file,const char* wroteTreeName, const cutParams* cut);
void SetCutParams(cutParams* kineCut);

using std::ofstream;

/**
 * @brief Execute Acceptancy and Efficiency tests.
 * 
 * @param filename Name of file where to find the tree to process.
 * @param outputfilename Name of the root output file to save processed data.
 * @param cut Cut parameters
 */
void AccEff(const char* filename,const char* outputfilename, const char* configname)
{
    std::cout << "\nACCEPTANCY AND EFFICIENCY TEST\n";
    std::cout << "Reading input file: " << filename <<'\n';
    std::cout << "Writing to output file: " << outputfilename <<'\n';
    std::cout << "Cut configuration file: " << configname <<'\n';

    //input file
    TFile* file = OpenFile(filename,"READ");

    //output file
    TFile* outputfile = OpenFile(outputfilename, "RECREATE");

    cutParams cut;
    cut.deserialize(configname);

    if(!cut.isValid())
    {
        std::cerr << "Error: Invalid cut parameters.\n";
        return;
    }

    //copy cut config file
    CopyFile(configname,ReplaceExtension(outputfilename,".cutconf").data());

    //Run acceptancy test
    std::unique_ptr<AccEffAnalyzer> results =  AccEffTest(file,ONIATTREENAME,&cut);
    if (results==nullptr) return;
    results->Write(ReplaceExtension(outputfilename,""));

    outputfile->Close();
    file->Close();
    delete file;
    std::cout << "Success.\n TTrees wrote to '" << outputfilename<< "' root file\n";
    return;
}

/**
 * @brief Acceptancy test
 * 
 * @param file File where to get the tree to acceptancy test.
 * @param wroteTreeName Name of the acceptancy tree to write
 * @return Output data.
 */
std::unique_ptr<AccEffAnalyzer> AccEffTest(TFile *file,const char* wroteTreeName, const cutParams* cut)
{
    TTree *myTree = (TTree *)file->Get("hionia/myTree");
    if (myTree == nullptr)
    {
        std::cout << "hionia/myTree tree not found\n";
        return nullptr;
    }

    AccCutter* cutterAcc =new AccCutter();
    EffCutter* cutterEff =new EffCutter(cut);
    OniaWriter* writer =new OniaWriterBase(wroteTreeName,QQtype::Gen);
    OniaReader* reader=new OniaReader(myTree,true);
    std::unique_ptr<AccEffAnalyzer> outputer(new AccEffAnalyzer(reader,cutterAcc,cutterEff,writer));

    outputer->Test();
    return outputer;
}

#if !defined(__CLING__)

int main(int argc, char **argv)
{
    if (argc == 4)
        AccEff(argv[1],argv[2],argv[3]);
    else
    {
        if (argc == 1)
        {
            const char* defaultfilename = "../rootfiles/merged_HiForestAOD_MC.root";
            const char* defaultoutputfile = "../rootfiles/merged_HiForestAOD_MC_AccEff";
            const char* defaultconfig = "../rootfiles/merged_HiForestAOD_MC.cutconf";
            AccEff(defaultfilename,defaultoutputfile,defaultconfig);
        }
        else
        {
            std::cerr << "Incorrect number of parameters\n";  
        }
    }
    return 0;
}

#endif