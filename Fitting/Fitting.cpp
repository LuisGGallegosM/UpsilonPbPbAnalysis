
#include "Fitting.h"

void SetFitConfig(fitConfig* fitConf);

/**
 * @brief Does a invariant mass fit, from a branch in a tree
 * in a root file. Saves the plot in a tree and in pdf file format.
 * 
 * @param filename filename of the root file where to find the tree
 * @param outfilename filename of root file where to store fit results
 * @param config fit configuration parameters
 */
void Fitting(const char* filename, const char* outfilename, const char* configname)
{
    std::cout << "\nFITTING\n";
    std::cout << "Reading input file: " << filename <<'\n';
    std::cout << "Writing output file: " << outfilename <<'\n';
    std::cout << "Reading configuration file: " << configname <<'\n';

    TFile file(filename, "READ");

    if (file.IsZombie()) 
    {
        std::cerr << "file "<< filename <<" cannot be read\n";
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
    CopyFile(ReplaceExtension(filename,".cutconf").data(),ReplaceExtension(outfilename,".cutconf").data() );

    
    RooAbsReal* fittedFunc = massFitter->fit();
    
    fitParams fParams = massFitter->getFitParams();

    fParams.serialize(ReplaceExtension(outfilename,".fit").data());

    massFitter->getResults()->Print();

    newfile.cd();
    massFitter->getDataset()->Write();
    massFitter->getResults()->Write("fitresults");
    fittedFunc->Write();
    massFitter->getVar()->Write();
}

#if !defined(__CLING__)

int main(int argc, char **argv)
{
    if (argc ==4)
    {
        Fitting(argv[1],argv[2],argv[3]);
    }
    else
    {
        std::cerr << "Incorrect number of parameters\n";  
    }
        
    return 0;
}

#endif