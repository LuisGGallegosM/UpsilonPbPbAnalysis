
#include "EffAnalyzer.h"

std::unique_ptr<EffAnalyzer> createEffAnalyzer(TTree* input,CutParams* effCut, const char* outTreeName )
{
    if (effCut->getIsMC())
        return std::unique_ptr<EffAnalyzer>(new EffAnalyzerMC(input,effCut,outTreeName));
    else
        return nullptr;
}