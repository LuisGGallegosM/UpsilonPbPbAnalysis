
#include "utils.h"
#include <fstream>

std::string ReplaceExtension(const char* outfilename,const char* newextension)
{
    std::string outputfilenamestr=std::string(outfilename);
    std::string newextname = outputfilenamestr.substr(0,outputfilenamestr.find_last_of('.'));
    return newextname + newextension;
}

void CopyFile(const char* inputpath, const char* outputpath)
{
    std::ofstream outputfile(outputpath);
    std::ifstream inputfile(inputpath);
    outputfile << inputfile.rdbuf();
    return;
}

void getFitParams(fitParams& fParams, const RooRealVar* var, const RooAbsReal* fittedFunc, bool bkgOn)
{
    RooArgSet* params= fittedFunc->getParameters(*var);

    fParams.dcb.alpha=   params->getRealValue("alpha_Y1S_1");
    fParams.dcb.f =       params->getRealValue("f_Y1S");
    fParams.dcb.mean =    params->getRealValue("mean_Y1S_1");
    fParams.dcb.n =       params->getRealValue("n_Y1S_1");
    fParams.dcb.sigma1 =   params->getRealValue("sigma_Y1S_1");
    fParams.dcb.x =       params->getRealValue("x_Y1S");
    fParams.nSig =      params->getRealValue("nSig_Y1S");

    if (bkgOn)
    {
        fParams.nBkg =      params->getRealValue("nBkg");
        fParams.chk4_k1 =      params->getRealValue("chk4_k1");
        fParams.chk4_k2 =      params->getRealValue("chk4_k2");
    }
}