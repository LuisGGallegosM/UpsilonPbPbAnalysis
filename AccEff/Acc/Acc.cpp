

#include "TTree.h"
#include "TFile.h"

#include "../../Utils/Helpers/Helpers.h"
#include "AccAnalyzer.h"

#include <iostream>

/**
 * @brief Acceptancy test function
 * 
 * @param filename root file path for input.
 * @param outputfilename output root file path with passing onia.
 * @param configname 
 */
void AccTest(const char* filename,const char* outputfilename, const char* yieldfitfuncFilename)
{
    std::cout << "\nACCEPTANCY TEST\n";

    //input file
    std::cout << "Reading input file: " << filename <<'\n';
    TFile* file = OpenFile(filename,"READ");

    RooAbsReal* yieldfitFunc=nullptr;
    if(yieldfitfuncFilename!=nullptr)
    {
        std::cout << "Reading yield weight function input file: " << yieldfitfuncFilename <<'\n';
        TFile* yieldFitFile = OpenFile(yieldfitfuncFilename,"READ");
        yieldfitFunc = dynamic_cast<RooAbsReal*>( yieldFitFile->Get(yieldFitFuncName) );
    }
    
    //output file
    std::string outfilename=outputfilename;
    std::cout << "Writing to output file: " << outfilename <<'\n';
    TFile* outputfile = OpenFile(outfilename.data(), "RECREATE");

    TTree *myTree = GetTree(file,"hionia/myTree");

    AccAnalyzer accAnalyzer(myTree,"DetectableOnia",yieldfitFunc);

    //Run acceptancy test
    accAnalyzer.Test();

    //write plots to same folder as outputfilename and prefixed with outputfilename
    std::string outputfilesBasename=ReplaceExtension(outfilename.data(),"");
    accAnalyzer.Write(outputfilesBasename);

    outputfile->Close();
    file->Close();
    delete file;
    delete outputfile;
    std::cout << "Success.\n TTrees wrote to '" << outfilename<< "' root file\n";
}