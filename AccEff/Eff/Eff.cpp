
#include <iostream>

#include "TTree.h"
#include "TFile.h"

#include "../../Utils/Helpers/Helpers.h"
#include "../../OniaBase/Params/Params.h"
#include "../Common/AccEffAux.h"
#include "EffAnalyzer.h"

/**
 * @brief Efficiency test
 * 
 * @param filename input path to root file for efficiency test
 * @param outputfilename output path to root file name
 * @param configname cut configuration file used for quality cuts.
 */
void EffTest(const char* filename,const char* outputfilename, const char* configname,  const char* yieldfitfuncFilename)
{
    std::cout << "\nEFFICIENCY TEST\n";

    //input file
    std::cout << "Reading input file: " << filename <<'\n';
    TFile* file = OpenFile(filename,"READ");

    std::cout << "Reading cutconf file: " << configname <<'\n';

    RooAbsReal* yieldfitFunc=nullptr;
    if(yieldfitfuncFilename!=nullptr)
    {
        std::cout << "Reading yield weight function input file: " << yieldfitfuncFilename <<'\n';
        TFile* yieldFitFile = OpenFile(yieldfitfuncFilename,"READ");
        yieldfitFunc = dynamic_cast<RooAbsReal*>( yieldFitFile->Get(yieldFitFuncName) );
    }
    WeightFuncRooAbs yieldfit(yieldfitFunc);
    WeightFunc* yieldfitPtr = (yieldfitFunc==nullptr) ? nullptr : &yieldfit;

    //read cut parameters
    CutParams cut;
    cut.deserialize(configname);
    if(!cut.isValid())
    {
        std::cerr << "Error: Invalid cut parameters.\n";
        return;
    }
    
    //output file
    std::string outfilename= outputfilename;
    std::cout << "Writing to output file: " << outfilename <<'\n';
    TFile* outputfile = OpenFile(outfilename.data(), "RECREATE");

    TTree *myTree = GetTree(file,"hionia/myTree");

    std::unique_ptr<EffAnalyzer> effAnalyzer = createEffAnalyzer(myTree,&cut,"RecoCutOnia",yieldfitPtr);

    //write results to same folder as outputfilename
    std::string outputfilesBasename=ReplaceExtension(outfilename.data(),"");
    //Run efficiency test
    effAnalyzer->Test(outputfilesBasename);
    effAnalyzer->getHists()->Write();
    //clean up
    outputfile->Close();
    file->Close();
    delete file;
    delete outputfile;
    std::cout << "Success.\n TTrees wrote to '" << outfilename<< "' root file\n";
    return;
}
