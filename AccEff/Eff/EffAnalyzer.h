#ifndef EFFANALYZER
#define EFFANALYZER

#include "TH2.h"
#include "TEfficiency.h"

#include "../../OniaBase/TreeProcessor/TreeProcessor.h"
#include "../../OniaBase/OniaIO/OniaIO.h"
#include "EffCutter.h"

class EffAnalyzer
{
    public:
    virtual void Write(const std::string& basename) = 0;
    virtual void Test() = 0;

    virtual ~EffAnalyzer() = default;
};

class EffHistografer
{
    TH2F* etaVsPtQQRecoCut;
    TH2F* etaVsPtMuRecoCut;
    TH2F* etaVsPtQQDet;
    TH1F* ptHistQQRecoCut;
    TH1F* ptHistQQDet;
    std::unique_ptr<TEfficiency> ptQQEfficiency;
    std::unique_ptr<TEfficiency> etaVsPtQQEfficiency;

    public:
    struct inputs
    {
        float pT;
        float y;
        float etaMuPl;
        float etaMuMi;
        float ptMuPl;
        float ptMuMi;
    };
    EffHistografer();

    void FillDet(float y, float pT);
    void FillRecoCut(const inputs* input);
    void Write(const std::string& basename);
};

template<typename Reader>
class EffAnalyzerBase : public TreeProcessor, public EffAnalyzer
{
    EffHistografer hists;
    OniaReader<Reader> oniaReader;
    EffCutter effCutter;
    OniaWriterRecoQQ oniaWriter;
    std::unique_ptr<AccCutter> accCutter;

    void CaptureRecoQQ(Int_t index, Long64_t entry)
    {
        auto input = oniaReader.getData();
        EffHistografer::inputs data = extractData(input,index);

        hists.FillRecoCut(&data);
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
    EffAnalyzerBase(TTree* input,CutParams* effCut, const char* outTreeName ): 
        TreeProcessor(input,outTreeName),oniaReader(),oniaWriter(),effCutter(effCut)
    {
        registerInputs(&oniaReader);
        registerOutputs(&oniaWriter);
    }

    void Write(const std::string& basename) override
    {
        TreeProcessor::write();
        hists.Write(basename);
    }

    void Test() override { Process(); }

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

template<typename Data>
EffHistografer::inputs extractData( const Data* input ,int index)
{
    EffHistografer::inputs data;

    TLorentzVector* mom4vec=(TLorentzVector*) input->recoQQ.mom4->At(index);

    int mupl_idx = input->recoQQ.mupl_idx[index];//plus muon index
    int mumi_idx = input->recoQQ.mumi_idx[index];//minus muon index
    TLorentzVector* mom4vecPl=(TLorentzVector*) input->recoMu.mom4->At(mupl_idx);
    TLorentzVector* mom4vecMi=(TLorentzVector*) input->recoMu.mom4->At(mumi_idx);

    data.pT = mom4vec->Pt();
    data.y = fabs(mom4vec->Rapidity());
    data.etaMuPl=fabs(mom4vecPl->Eta());
    data.etaMuMi=fabs(mom4vecMi->Eta());
    data.ptMuPl=mom4vecPl->Pt();
    data.ptMuMi=mom4vecMi->Pt();
    return data;
}

using EffAnalyzerRealData = EffAnalyzerBase<OniaRealData>;
using EffAnalyzerMC = EffAnalyzerBase<OniaMCData>;

std::unique_ptr<EffAnalyzer> createEffAnalyzer(TTree* input,CutParams* effCut, const char* outTreeName );

#endif