#ifndef SUPERMULTIFIT
#define SUPERMULTIFIT

#include"../../OniaBase/Params/Params.h"

void Supermultifit(const char** multifitpaths,const char* outputpath,int size);

#if defined(__CLING__)
#include "LLRtest.cpp"
#endif

#endif