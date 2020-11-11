
#include "Fitting.h"

void SetFitConfig(fitConfig* fitConf);
void getFitParams(fitParams& fParams, const RooRealVar* var, const RooAbsReal* fittedFunc, bool bkgOn);
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

    OniaMassFitter massFitter(tree_skimmed, &config);

    //copy fit config file, same filename as output root file but with .cutconf extension
    CopyFile(configname, ReplaceExtension(outfilename,".fitconf").data());
    CopyFile(ReplaceExtension(filename,".cutconf").data(),ReplaceExtension(outfilename,".cutconf").data() );

    fitParams fParams;
    RooAbsReal* fittedFunc = massFitter.fit();

    getFitParams(fParams, massFitter.getVar(), fittedFunc,config.bkgOn);
    fParams.serialize(ReplaceExtension(outfilename,".fit").data());

    massFitter.getResults()->Print();

    newfile.cd();
    massFitter.getDataset()->Write();
    massFitter.getResults()->Write("fitresults");
    fittedFunc->Write();
    massFitter.getVar()->Write();
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

void getFitParams(fitParams& fParams, const RooRealVar* var, const RooAbsReal* fittedFunc, bool bkgOn)
{
    RooArgSet* params= fittedFunc->getParameters(*var);

    fParams.dcb.alpha=   params->getRealValue("alpha_1");
    fParams.dcb.f =       params->getRealValue("f");
    fParams.dcb.mean =    params->getRealValue("mean_1");
    fParams.dcb.n =       params->getRealValue("n_1");
    fParams.dcb.sigma1 =   params->getRealValue("sigma_1");
    fParams.dcb.x =       params->getRealValue("x");
    fParams.nSig =      params->getRealValue("nSig");

    if (bkgOn)
    {
        fParams.nBkg =      params->getRealValue("nBkg");
        fParams.chk4_k1 =      params->getRealValue("chk4_k1");
        fParams.chk4_k2 =      params->getRealValue("chk4_k1");
    }
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