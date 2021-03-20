

#include "TTree.h"
#include "TFile.h"

#include "../../Utils/utils.h"
#include "AccAnalyzer.h"

#include <iostream>

/**
 * @brief Acceptancy test
 * 
 * @param file File where to get the tree to acceptancy test.
 * @param wroteTreeName Name of the acceptancy tree to write
 * @return Output data.
 */
void AccTest(const char* filename,const char* outputfilename, const char* configname)
{
    std::cout << "\nACCEPTANCY TEST\n";

    //input file
    std::cout << "Reading input file: " << filename <<'\n';
    TFile* file = OpenFile(filename,"READ");
    
    //output file
    std::string outfilename=outputfilename;
    std::cout << "Writing to output file: " << outfilename <<'\n';
    TFile* outputfile = OpenFile(outfilename.data(), "RECREATE");

    TTree *myTree = GetTree(file,"hionia/myTree");

    AccAnalyzer accAnalyzer(myTree,"DetectableOnia");

    //Run acceptancy test
    accAnalyzer.Test();

    //write results to same folder as outputfilename
    std::string outputfilesBasename=ReplaceExtension(outfilename.data(),"");
    accAnalyzer.Write(outputfilesBasename);

    outputfile->Close();
    file->Close();
    delete file;
    delete outputfile;
    std::cout << "Success.\n TTrees wrote to '" << outfilename<< "' root file\n";
}