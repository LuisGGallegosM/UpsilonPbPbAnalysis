
#include <iostream>

#include "TTree.h"
#include "TFile.h"

#include "../../Utils/Helpers/Helpers.h"
#include "../../OniaBase/Params/Params.h"
#include "EffAnalyzer.h"

/**
 * @brief Efficiency test
 * 
 * @param filename input path to root file for efficiency test
 * @param outputfilename output path to root file name
 * @param configname cut configuration file used for quality cuts.
 */
void EffTest(const char* filename,const char* outputfilename, const char* configname)
{
    std::cout << "\nEFFICIENCY TEST\n";

    //input file
    std::cout << "Reading input file: " << filename <<'\n';
    TFile* file = OpenFile(filename,"READ");
    
    //output file
    std::string outfilename= outputfilename;
    std::cout << "Writing to output file: " << outfilename <<'\n';
    TFile* outputfile = OpenFile(outfilename.data(), "RECREATE");

    TTree *myTree = GetTree(file,"hionia/myTree");

    //read cut parameters
    CutParams cut;
    cut.deserialize(configname);
    if(!cut.isValid())
    {
        std::cerr << "Error: Invalid cut parameters.\n";
        return;
    }

    std::unique_ptr<EffAnalyzer> effAnalyzer = createEffAnalyzer(myTree,&cut,"RecoCutOnia");

    //Run efficiency test
    effAnalyzer->Test();

    //write results to same folder as outputfilename
    std::string outputfilesBasename=ReplaceExtension(outfilename.data(),"");
    effAnalyzer->Write(outputfilesBasename);

    //clean up
    outputfile->Close();
    file->Close();
    delete file;
    delete outputfile;
    std::cout << "Success.\n TTrees wrote to '" << outfilename<< "' root file\n";
    return;
}
