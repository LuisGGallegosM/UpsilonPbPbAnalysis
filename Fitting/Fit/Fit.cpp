

#include "Fit.h"

/**
 * @brief Does a invariant mass fit, from a branch in a tree
 * in a root file. Saves the plot in a tree and in pdf file format.
 * 
 * @param inputfilename filename of the root file where to find the tree
 * @param outfilename filename of root file where to store fit results
 * @param config fit configuration parameters
 */
void Fit(const char* inputfilename, const char* cutfilename, const char* outfilename, const char* configname)
{
    std::cout << "\nFITTING\n";
    std::cout << "Reading input file: " << inputfilename <<'\n';
    std::cout << "Reading input file: " << cutfilename <<'\n';
    std::cout << "Reading input file: " << configname <<'\n';
    std::cout << "Writing output file: " << outfilename <<'\n';

    TFile file(inputfilename, "READ");

    if (file.IsZombie()) 
    {
        std::cerr << "file "<< inputfilename <<" cannot be read\n";
        return;
    }

    TFile newfile(outfilename,"CREATE");

    if (newfile.IsZombie()) 
    {
        std::cerr << "file "<< outfilename<<" cannot be wrote or already exists\n";
        return;
    }

    fitConfig config;
    config.deserialize(configname);

    if(!config.isValid())
    {
        std::cerr << "Error: Invalid arguments\n";
        return;
    }

    TTree *tree_skimmed = (TTree *)file.Get(ONIATTREENAME);

    std::unique_ptr<OniaMassFitter> massFitter;

    //fit with first three Upsilon or only the first
    if (config.isMoreUpsilon())
    {
        massFitter.reset(new OniaMassFitter2(tree_skimmed, &config));
    }
    else
    {
        massFitter.reset(new OniaMassFitter(tree_skimmed, &config));
    }

    //copy fit config file, same filename as output root file but with .cutconf extension
    CopyFile(configname, ReplaceExtension(outfilename,".fitconf").data());
    CopyFile(cutfilename,ReplaceExtension(outfilename,".cutconf").data() );

    
    RooAbsReal* fittedFunc = massFitter->fit();
    
    fitParamsWithErrors fParams = massFitter->getFitParams();

    fParams.serialize(ReplaceExtension(outfilename,".fit").data());

     assert(fParams.isValid());

    massFitter->getResults()->Print();

    newfile.cd();
    massFitter->getDataset()->Write();
    massFitter->getResults()->Write("fitresults");
    fittedFunc->Write();
    massFitter->getVar()->Write("mass");
}

