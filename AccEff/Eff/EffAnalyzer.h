#ifndef EFFANALYZER
#define EFFANALYZER

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
class EffAnalyzerBase : public TreeProcessor, public EffAnalyzer
{
    EffHistografer hists;
    OniaReader<Reader> oniaReader;
    EffCutter effCutter;
    OniaWriterRecoQQ oniaWriter;
    std::unique_ptr<AccCutter> accCutter;
    bool corrected;

    void CaptureRecoQQ(Int_t index, Long64_t entry)
    {
        auto input = oniaReader.getData();
        EffHistografer::inputs data = extractRecoCut(input,index);

        float weight=1.0f;
        if (corrected)
            weight=calculateTnp(data.ptMuPl,data.ptMuMi,data.yMuPl,data.yMuMi);

        hists.FillRecoCut(&data,weight);
        oniaWriter.writeData(input,SimpleSelector{entry,index});
        FillEntries();
    }

    void CaptureDetQQ(Int_t index, Long64_t entry)
    {
        TLorentzVector* mom4vec=(TLorentzVector*) oniaReader.getData()->genQQ.mom4->At(index);
        float pT = mom4vec->Pt();
        float y = fabs(mom4vec->Rapidity());
        hists.FillDet(y,pT);
    }

    public:
    EffAnalyzerBase(TTree* input,CutParams* effCut, const char* outTreeName, bool corr=false ): 
        TreeProcessor(input,outTreeName),oniaReader(),oniaWriter(),effCutter(effCut),corrected(corr)
    {
        registerInputs(&oniaReader);
        registerOutputs(&oniaWriter);
    }

    void Write(const std::string& basename) override
    {
        TreeProcessor::write();
        hists.Write(basename);
    }

    void Test() override 
    { 
        Process(); 
    }

    void ProcessEvent(Long64_t entry) override
    {
        int size=oniaReader.getData()->genQQ.size;
        
        for(int index=0;index<size;++index)
        {
            auto input=oniaReader.getData();
            if (accCutter->cut(input,index,entry))
            {
                CaptureDetQQ(index,entry);
                int recoQQindex= input->which.GenQQWhichReco[index];
                if (recoQQindex>=0)
                {
                    if (effCutter.cut(input,recoQQindex,entry))
                        CaptureRecoQQ(recoQQindex,entry);
                }
            }
        }
    }
};

using EffAnalyzerRealData = EffAnalyzerBase<OniaRealData>;
using EffAnalyzerMC = EffAnalyzerBase<OniaMCData>;

std::unique_ptr<EffAnalyzer> createEffAnalyzer(TTree* input,CutParams* effCut, const char* outTreeName, bool corr=false );

#endif