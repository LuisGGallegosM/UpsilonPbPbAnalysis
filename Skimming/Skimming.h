#ifndef SKIMMING
#define SKIMMING

#include "TTree.h"
#include "TFile.h"

#include "../Params.h"
#include "../Utils/serialize.h"
#include "../Utils/utils.h"
#include "OniaSkimmer.h"
#include "OniaCutter.h"
#include "JetSkimmer.h"
#include "JetCutter.h"

void Skimming(const char* filename,const char* outputfilename);

#if defined(__CLING__)
#include "OniaSkimmer.cpp"
#include "OniaCutter.cpp"
#include "JetSkimmer.cpp"
#include "JetCutter.cpp"
#include "../Utils/serialize.cpp"
#include "../Utils/utils.cpp"
#endif

#endif