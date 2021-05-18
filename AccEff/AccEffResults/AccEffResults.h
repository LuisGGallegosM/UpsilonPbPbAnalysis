#ifndef ACCEFFRESULTS
#define ACCEFFRESULTS

const char AnalysisNoteName[] ="Physics Letters B 790 (2019) 270–293";
const char xAxisLabel[] ="p^{#mu#mu}_{T} GeV/c";

void AccEffResults(const char* accFilename, const char* effFilename, const char* fitFileRealDataPath, const char* fitFileMCPath, const char* outputname);

#endif