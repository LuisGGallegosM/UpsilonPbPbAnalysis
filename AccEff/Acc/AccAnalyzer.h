#ifndef ACCANALYZER
#define ACCANALYZER

#include "RooAbsReal.h"

#include "../../OniaBase/TreeProcessor/TreeProcessor.h"
#include "../../OniaBase/OniaIO/OniaIO.h"
#include "../../OniaBase/Params/Params.h"
#include "../Common/AccEffAux.h"
#include "../Common/AccCutter.h"
#include "AccHistografer.h"

class AccAnalyzer
{
    private:
    AccHistografer hists;
    OniaReader<OniaGenOnlyData> oniaReader;
    AccCutter accCutter;
    WeightFunc* weightFunc;
    bool onlyNumCorr;

    public:
    AccAnalyzer(TTree* input,const char* outTreeName, WeightFunc* weightFunc=nullptr, bool onlyNCorr =false);

    const AccHistografer* getHists() const { return &hists;}

    void Test(const std::string& basename) 
    { 
        TreeProcess(this,oniaReader.getName(),oniaReader.getEntries()); 
        hists.finalCalculations(basename);
    }
    void ProcessEvent(Long64_t entry);
};

#endif