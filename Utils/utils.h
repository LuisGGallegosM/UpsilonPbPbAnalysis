#ifndef UTILS
#define UTILS

#include <string>
#include "Params.h"
#include "RooRealVar.h"
#include "RooAbsReal.h"

std::string ReplaceExtension(const char* outfilename,const char* newextension);

void CopyFile(const char* inputpath, const char* outputpath);

void getFitParams(fitParams& fParams, const RooRealVar* var, const RooAbsReal* fittedFunc, bool bkgOn);

#if defined(__CLING__)
#include "../Utils/utils.cpp"
#endif

#endif