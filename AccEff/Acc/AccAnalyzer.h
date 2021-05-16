#ifndef ACCANALYZER
#define ACCANALYZER

#include "RooAbsReal.h"

#include "../../OniaBase/TreeProcessor/TreeProcessor.h"
#include "../../OniaBase/OniaIO/OniaIO.h"
#include "../Common/AccEffAux.h"
#include "../Common/AccCutter.h"
#include "AccHistografer.h"

class AccAnalyzer
{
    private:
    AccHistografer hists;
    OniaReader<OniaGenOnlyData> oniaReader;
    AccCutter accCutter;
    RooAbsReal* weightFunc;

    void Analyze(const OniaGenOnlyData* input,Int_t index, Long64_t entry);

    public:
    AccAnalyzer(TTree* input,const char* outTreeName, RooAbsReal* weightFunc=nullptr);

    void Write(const std::string& basename);

    void Test() { TreeProcess(this,oniaReader.getName(),oniaReader.getEntries()); }
    void ProcessEvent(Long64_t entry);
};

#endif