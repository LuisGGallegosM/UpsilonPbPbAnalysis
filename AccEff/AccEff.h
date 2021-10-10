#ifndef ACCEFF
#define ACCEFF

void AccTest(const char* filename,const char* outputfilename, const char* yieldfitfuncFilename);
void EffTest(const char* filename,const char* outputfilename, const char* configname,  const char* yieldfitfuncFilename);
#include "AccEffResults/AccEffResults.h"
#include "YieldFit/YieldFit.h"
#include "ClosureTest/ClosureTest.h"
#include "ClosureTest/ClosureTestJet.h"

#endif