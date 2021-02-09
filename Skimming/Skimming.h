#ifndef SKIMMING
#define SKIMMING

#include "TTree.h"
#include "TFile.h"

#include "../Utils/Params.h"
#include "../Utils/serialize.h"
#include "../Utils/utils.h"
#include "OniaSkimmer.h"

void Skimming(const char* filename,const char* outputfilename);

#endif