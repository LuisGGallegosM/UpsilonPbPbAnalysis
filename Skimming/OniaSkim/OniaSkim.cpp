
#include <iostream>

#include "TTree.h"
#include "TFile.h"

#include "../../Utils/Helpers/Helpers.h"
#include "../../OniaBase/Common/Common.h"

#include "OniaSkimmer.h"

/**
 * @brief Execute Upsilon skimming
 * 
 * @param filename Path to the root input file.
 * @param outputfilename Path to the root output file to save skimmed data.
 * @param configname Path to the cutconf file containing Cut parameters.
 */
void OniaSkim(const char* filename,const char* outputfilename, const char* configname)
{
    std::cout << "\nSKIMMING ONIA\n";
    std::cout << "Reading input file: " << filename <<'\n';
    std::cout << "Reading Cut configuration file: " << configname <<'\n';
    std::cout << "Writing output file: " << outputfilename <<'\n';

    //input file
    TFile* file = OpenFile(filename,"READ");

    //output file
    TFile* outputfile = OpenFile(outputfilename, "CREATE");

    //cut params
    CutParams cut;
    cut.deserialize(configname);

    if(!cut.isValid())
    {
        std::cerr << "Error: Invalid cut parameters.\n";
        return;
    }

    //copy cut config file to same directory as outputfile with output file same name but .cutconf
    CopyFile(configname,ReplaceExtension(outputfilename,".cutconf").data());

    //get tree to skim
    TTree *myTree = GetTree(file,"hionia/myTree");

    //execute skim
    std::unique_ptr<Skimmer> skimmer = createOniaSkimmer(myTree,&cut,oniaTreeName);
    skimmer->Skim();

    //write tree
    skimmer->Write();
    std::cout << "Success.\n TTrees wrote to '" << outputfilename<< "' root file\n";

    //clean up
    outputfile->Close();
    file->Close();
    delete outputfile;
    delete file;
    
    return;
}