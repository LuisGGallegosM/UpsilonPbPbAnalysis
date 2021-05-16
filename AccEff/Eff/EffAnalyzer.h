#ifndef EFFANALYZER
#define EFFANALYZER

#include"RooAbsReal.h"

#include "../../OniaBase/TreeProcessor/TreeProcessor.h"
#include "../../OniaBase/OniaIO/OniaIO.h"
#include "EffCutter.h"
#include "EffHistografer.h"

float calculateTnp(float muplPt, float mumiPt, float muplY, float mumiY);

class EffAnalyzer
{
    public:
    virtual void Write(const std::string& basename) = 0;
    virtual void Test() = 0;

    virtual ~EffAnalyzer() = default;
};

template<typename Reader>
class EffAnalyzerBase : public EffAnalyzer
{
    EffHistografer hists;
    OniaReader<Reader> oniaReader;
    EffCutter effCutter;
    std::unique_ptr<AccCutter> accCutter;
    RooAbsReal* weightFunc;

    void CaptureGenQQ(const Reader* input,Int_t index, Long64_t entry)
    {
        EffHistografer::inputs data = extractGen(input,index);

        float weight=1.0f;
        if (weightFunc!=nullptr)
        {
            weight=calculateTnp(data.ptMuPl,data.ptMuMi,data.yMuPl,data.yMuMi);
            weightFunc->getVariables()->setRealValue("pt",data.pT);
            weight*=weightFunc->getVal();
        }
        
        hists.FillRecoCut(&data,weight);
    }

    void CaptureDetQQ(const Reader* input,Int_t index, Long64_t entry)
    {
        TLorentzVector* mom4vec=(TLorentzVector*) input->genQQ.mom4->At(index);
        float pT = mom4vec->Pt();
        float y = fabs(mom4vec->Rapidity());
        float weight = 1.0f;
        if (weightFunc!=nullptr)
        {
            weightFunc->getVariables()->setRealValue("pt",pT);
            weight=weightFunc->getVal();
        }
        hists.FillDet(y,pT,weight);
    }

    public:
    EffAnalyzerBase(TTree* input,CutParams* effCut, const char* outTreeName, RooAbsReal* weights=nullptr): 
        oniaReader(input),effCutter(effCut),weightFunc(weights)
    {
    }

    void Write(const std::string& basename) override
    {
        hists.Write(basename);
    }

    void Test() override 
    { 
        TreeProcess( this,oniaReader.getName(),oniaReader.getEntries());
    }

    void ProcessEvent(Long64_t entry)
    {
        auto input = oniaReader.getData(entry);
        int size=input->genQQ.size;
        
        for(int index=0;index<size;++index)
        {
            if (accCutter->cut(input,index,entry))
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
};

using EffAnalyzerRealData = EffAnalyzerBase<OniaRealData>;
using EffAnalyzerMC = EffAnalyzerBase<OniaMCData>;

std::unique_ptr<EffAnalyzer> createEffAnalyzer(TTree* input,CutParams* effCut, const char* outTreeName, RooAbsReal* weights );
#endif