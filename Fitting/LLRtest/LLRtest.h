#ifndef LLRTEST
#define LLRTEST

#include"../../OniaBase/Params/Params.h"

void LLRtest(const char** multifitpaths,const char* outputpath,int size);

#if defined(__CLING__)
#include "LLRtest.cpp"
#endif

#endif