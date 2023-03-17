#ifndef ACCEFF
#define ACCEFF

#include "AccEffResults/AccEffResults.h"
#include "YieldFit/YieldFit.h"
#include "ClosureTest/ClosureTest.h"
#include "ClosureTest/ClosureTestJet.h"
#include "AddWeights/AddWeights.h"
#include "AddWeights/AddWeights1D.h"
#include "CrossSection/CrossSectionCalculate.h"
#include "Unfolding/Unfold_data.h"
#include "Unfolding/Unfold_mc.h"

void AccTest(const char* filename,const char* outputfilename, const char* yieldfitfuncFilename);
void EffTest(const char* filename,const char* outputfilename, const char* configname,  const char* yieldfitfuncFilename);

#endif