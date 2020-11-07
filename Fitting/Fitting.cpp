
#include "Fitting.h"

void copyCutFile(const char* filename, const char* outfilename);
void SetFitConfig(fitConfig* fitConf);
std::string ReplaceExtension(const char* outfilename,const char* newextension);

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
    TFile file(filename, "READ");

    TTree *tree_skimmed = (TTree *)file.Get(ONIATTREENAME);

    fitConfig config;
    config.deserialize(configname);

    if(!config.isValid())
    {
        std::cerr << "Error: Invalid arguments\n";
        return;
    }
    
    OniaMassFitter massFitter(tree_skimmed, &config);

    TFile newfile(outfilename,"RECREATE");

    //redirect cout
    std::ofstream logFile(ReplaceExtension(outfilename,".log"));
    std::cout.rdbuf(logFile.rdbuf());

    //copy fit config file, same filename as output root file but with .cutconf extension
    std::ofstream( ReplaceExtension(outfilename,".fitconf")) << std::ifstream(configname).rdbuf();

    RooAbsReal* fittedFunc = massFitter.fit();

    massFitter.getResults()->Print();

    newfile.cd();
    massFitter.getDataset()->Write();
    massFitter.getResults()->Write("fitresults");
    fittedFunc->Write();
    massFitter.getVar()->Write();
}

std::string ReplaceExtension(const char* outfilename,const char* newextension)
{
    std::string outputfilenamestr=std::string(outfilename);
    std::string newextname = outputfilenamestr.substr(0,outputfilenamestr.find_last_of('.'));
    return newextname + newextension;
}


void SetFitConfig(fitConfig* fitConf)
{
    //set background and fit range
    fitConf->bkgOn =false;
    fitConf->massHigh = 10.0f;
    fitConf->massLow = 8.5f;

    //set initial values for fitting parameters
    fitConf->initialValues.nSig = 5000000.0f;
    fitConf->initialValues.nBkg = 500000.0f;
    fitConf->initialValues.chk4_k1=0.2f;
    fitConf->initialValues.chk4_k2=-0.1f;

    fitConf->initialValues.dcb.mean=9.46f;
    fitConf->initialValues.dcb.alpha = 2.522f;
    fitConf->initialValues.dcb.n =1.705f;
    fitConf->initialValues.dcb.sigma1 = 0.1f;
    fitConf->initialValues.dcb.x = 0.5f;
    fitConf->initialValues.dcb.f = 0.5f;
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