#ifndef ACCEFF
#define ACCEFF

void AccTest(const char* filename,const char* outputfilename, const char* yieldfitfuncFilename);
void EffTest(const char* filename,const char* outputfilename, const char* configname, bool corr);
#include "AccEffResults/AccEffResults.h"
#include "YieldFit/YieldFit.h"
#include "ClosureTest/ClosureTest.h"

#endif