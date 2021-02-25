#ifndef FITTING
#define FITTING

#include <iostream>
#include <fstream>

#include "../Utils/utils.h"
#include "../OniaBase/FitParams.h"
#include "../OniaBase/OniaBase.h"
#include "OniaMassFit.h"

#include "TTree.h"
#include "TFile.h"

void Fitting(const char* filename, const char* outfilename);

#endif