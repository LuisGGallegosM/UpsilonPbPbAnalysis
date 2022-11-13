#ifndef SUPERMULTIFIT
#define SUPERMULTIFIT

#include"../../OniaBase/Params/Params.h"

void Supermultifit(const char** multifitpaths,const char* outputpath,int size);
void MultifitFileExpand(const char *multifitinput, const char *outputpath, const char* varname, int numvars, const char** vars);

#if defined(__CLING__)
#include "LLRtest.cpp"
#endif

#endif