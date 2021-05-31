#ifndef EFFANALYZER
#define EFFANALYZER

#include"RooAbsReal.h"

#include "../../OniaBase/TreeProcessor/TreeProcessor.h"
#include "../../OniaBase/OniaIO/OniaIO.h"
#include "../Common/AccEffAux.h"
#include "EffCutter.h"
#include "EffHistografer.h"

double calculateTnp(double muplPt, double mumiPt, double muplY, double mumiY);

class EffAnalyzer
{
    public:
    virtual const EffHistografer* getHists() const =0;
    virtual void Test(const std::string& basename) = 0;

    virtual ~EffAnalyzer() = default;
};

template<typename Reader>
class EffAnalyzerBase : public EffAnalyzer
{
    EffHistografer hists;
    OniaReader<Reader> oniaReader;
    EffCutter effCutter;
    AccCutter accCutter;
    WeightFunc* weightFunc;
    bool onlyNcorr;

    void CaptureGenQQ(const Reader* input,Int_t index, Long64_t entry)
    {
        EffHistografer::inputs data = extractGen(input,index);

        double weight=1.0;
        if (weightFunc!=nullptr)
        {
            weight= weightFunc->getWeight(data.pT);
            if (!onlyNcorr)
                weight*=calculateTnp(data.ptMuPl,data.ptMuMi,data.yMuPl,data.yMuMi);
        }
        
        hists.FillRecoCut(&data,weight);
    }

    void CaptureDetQQ(const Reader* input,Int_t index, Long64_t entry)
    {
        TLorentzVector* mom4vec=(TLorentzVector*) input->genQQ.mom4->At(index);
        double pT = mom4vec->Pt();
        double y = fabs(mom4vec->Rapidity());
        double weight = 1.0;
        if ((!onlyNcorr) && (weightFunc!=nullptr))
        {
            weight = weightFunc->getWeight(pT);
        }
        hists.FillDet(y,pT,weight);
    }

    public:
    EffAnalyzerBase(TTree* input,CutParams* effCut, const char* outTreeName, WeightFunc* weights=nullptr, bool tnpcorr=true): 
        oniaReader(input),effCutter(effCut),weightFunc(weights),onlyNcorr(tnpcorr)
    {
    }

    void Test(const std::string& basename) override 
    { 
        TreeProcess( this,oniaReader.getName(),oniaReader.getEntries());
        hists.finalCalculations(basename);
    }

    const EffHistografer* getHists() const override {return &hists;}

    void ProcessEvent(Long64_t entry)
    {
        auto input = oniaReader.getData(entry);
        int size=input->genQQ.size;
        
        for(int index=0;index<size;++index)
        {
            TLorentzVector* mom4vec=(TLorentzVector*) input->genQQ.mom4->At(index);
            if ((abs(mom4vec->Rapidity()) <= yMax) && ( mom4vec->Pt() <= ptMax ))
            {
                if (accCutter.cut(input,index,entry))
                {
                    CaptureDetQQ(input,index,entry);
                    int recoQQindex= input->which.GenQQWhichReco[index];
                    if (recoQQindex>=0)
                    {
                        if (effCutter.cut(input,recoQQindex,entry))
                            CaptureGenQQ(input,index,entry);
                    }
                }
            }
        }
    }
};

using EffAnalyzerRealData = EffAnalyzerBase<OniaRealData>;
using EffAnalyzerMC = EffAnalyzerBase<OniaMCData>;

std::unique_ptr<EffAnalyzer> createEffAnalyzer(TTree* input,CutParams* effCut, const char* outTreeName, WeightFunc* weights, bool onlyNumCorr =false );
#endif