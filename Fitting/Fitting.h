#ifndef FITTING
#define FITTING

#include <iostream>
#include <fstream>

#include "../Params.h"
#include "OniaMassFit.h"

#include "TTree.h"
#include "TFile.h"

void Fitting(const char* filename, const char* outfilename);

#if defined(__CLING__)
#include "OniaMassFit.cpp"
#endif

#endif