
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

    float alpha=    params->getRealValue("alpha_Y1S_1");
    float f =       params->getRealValue("f_Y1S");
    float mean =    params->getRealValue("mean_Y1S_1");
    float n =       params->getRealValue("n_Y1S_1");
    float sigma =   params->getRealValue("sigma_Y1S_1");
    float x =       params->getRealValue("x_Y1S");
    float nSigY1S = params->getRealValue("nSig_Y1S");
    float nSigY2S = params->getRealValue("nSig_Y2S");
    float nSigY3S = params->getRealValue("nSig_Y3S");

    float nBkg=0;
    float chk4_k1 = 0;
    float chk4_k2 = 0;

    if (bkgOn)
    {
        nBkg =      params->getRealValue("nBkg");
        chk4_k1 =      params->getRealValue("chk4_k1");
        chk4_k2 =      params->getRealValue("chk4_k2");
    }

    fParams.setNSig(nSigY1S,nSigY2S,nSigY3S);
    fParams.setNBkg(nBkg);
    fParams.setChk4(chk4_k1,chk4_k2);
    fParams.getDCBParams()->setParams(mean,alpha,n,sigma,x,f);
}