#ifndef FITTING
#define FITTING

#include <iostream>
#include <fstream>

#include "../Utils/utils.h"
#include "../Utils/Params.h"
#include "OniaMassFit.h"

#include "TTree.h"
#include "TFile.h"

void Fitting(const char* filename, const char* outfilename);

#if defined(__CLING__)
#include "OniaMassFit.cpp"
#include "../Utils/serialize.cpp"
#include "../Utils/utils.cpp"
#endif

#endif