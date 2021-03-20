#include "EffAnalyzer.h"
#include "../Common/AccEffAux.h"
#include "TCanvas.h"
#include "TPie.h"
#include <array>


template<typename Reader>
EffAnalyzerBase<Reader>::EffAnalyzerBase(TTree* input,CutParams* effCut, const char* outTreeName ) : 
    TreeProcessor(input,outTreeName),oniaReader(),oniaWriter(),effCutter(effCut)
{
    registerInputs(&oniaReader);
    registerOutputs(&oniaWriter);
    //initialize Histograms
    etaVsPtQQRecoCut =  createTH2QQ("y vs pt QQ Reco + Cut",    "p^{#mu#mu}_{t} vs |y^{#mu#mu}| QQ Reco + Cut");
    etaVsPtMuRecoCut =  createTH2Mu("eta vs pt Mu Reco + Cut",  "p^{#mu}_{t} vs |#eta^{#mu}| Mu Reco + Cut");
    etaVsPtQQDet =      createTH2QQ("y vs pt QQ Detectable",    "p^{#mu#mu}_{t} vs |y^{#mu#mu}| QQ Detectable");

    ptHistQQRecoCut=    createTH1(effNumName,"p^{#mu#mu}_{t} QQ Reco + Cut");
    ptHistQQDet=        createTH1(effDenName,"p^{#mu#mu}_{t} QQ Detectable");
}

template<typename Reader>
void EffAnalyzerBase<Reader>::ProcessEvent(Long64_t entry)
{
    Long64_t size=oniaReader.getData()->genQQ.size;
    
    for(Long64_t i=0;i<size;++i)
    {
        Analyze(i,entry);
    }
}

template<typename Reader>
void EffAnalyzerBase<Reader>::Write(const std::string& basename)
{
    TreeProcessor::write();

    //write 2D plots
    writeToCanvas(etaVsPtQQRecoCut,"|y^{#mu#mu}|","p^{#mu#mu}_{T} ( GeV/c )",basename+"_EtaPtQQ_RecoCut.pdf");
    writeToCanvas(etaVsPtMuRecoCut,"|#eta^{#mu}|"   ,"p^{#mu}_{T} ( GeV/c )",basename+"_EtaPtMu_RecoCut.pdf");

    //write 1D plots
    writeToCanvas(ptHistQQDet,      "p^{#mu#mu}_{T} ( GeV/c )", "N_{Det}^{#mu#mu}",basename+"_PtQQ_Det.pdf");
    writeToCanvas(ptHistQQRecoCut,  "p^{#mu#mu}_{T} ( GeV/c )", "N_{Rec}^{#mu#mu}",basename+"_PtQQ_RecoCut.pdf");

    etaVsPtQQRecoCut->Write(0,TObject::kOverwrite);
    etaVsPtMuRecoCut->Write(0,TObject::kOverwrite);
    ptHistQQDet->Write(0,TObject::kOverwrite);
    ptHistQQRecoCut->Write(0,TObject::kOverwrite);

    //calculate efficiency
    ptQQEfficiency = createTEff(ptHistQQRecoCut,ptHistQQDet,"pt QQ Efficiency");

    etaVsPtQQEfficiency = createTEff(etaVsPtQQRecoCut,etaVsPtQQDet,"eta vs pt QQ Efficiency");

    writeToCanvasEff(ptQQEfficiency.get(), "p^{#mu#mu}_{T} ( GeV/c )", "Eff",    basename+"_PtQQ_Efficiency.pdf");
    writeToCanvasEff2D(etaVsPtQQEfficiency.get(), "p^{#mu#mu}_{T} ( GeV/c )", "Eff",basename+"_EtaVsPtQQ_Efficiency.pdf");

    ptQQEfficiency->Write(0,TObject::kOverwrite);
    etaVsPtQQEfficiency->Write(0,TObject::kOverwrite);
}

template<typename Reader>
void EffAnalyzerBase<Reader>::Analyze(Int_t index, Long64_t entry)
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

template<typename Reader>
void EffAnalyzerBase<Reader>::CaptureDetQQ(Int_t index, Long64_t entry)
{
    TLorentzVector* mom4vec=(TLorentzVector*) oniaReader.getData()->genQQ.mom4->At(index);
    float pT = mom4vec->Pt();
    float y = fabs(mom4vec->Rapidity());

    etaVsPtQQDet->Fill(y,pT);
    ptHistQQDet->Fill(pT);
}

template<typename Reader>
void EffAnalyzerBase<Reader>::CaptureRecoQQ(Int_t index, Long64_t entry)
{
    auto input = oniaReader.getData();
    //read variables
    TLorentzVector* mom4vec=(TLorentzVector*) input->recoQQ.mom4->At(index);

    int mupl_idx = input->recoQQ.mupl_idx[index];//plus muon index
    int mumi_idx = input->recoQQ.mumi_idx[index];//minus muon index
    TLorentzVector* mom4vecPl=(TLorentzVector*) input->recoMu.mom4->At(mupl_idx);
    TLorentzVector* mom4vecMi=(TLorentzVector*) input->recoMu.mom4->At(mumi_idx);

    float pT = mom4vec->Pt();
    float y = fabs(mom4vec->Rapidity());

    float etaMuPl=fabs(mom4vecPl->Eta());
    float etaMuMi=fabs(mom4vecMi->Eta());
    float ptMuPl=mom4vecPl->Pt();
    float ptMuMi=mom4vecMi->Pt();

    ptHistQQRecoCut->Fill(pT);
    etaVsPtQQRecoCut->Fill(y,pT);
    etaVsPtMuRecoCut->Fill(etaMuPl,ptMuPl);
    etaVsPtMuRecoCut->Fill(etaMuMi,ptMuMi);
    oniaWriter.writeData(input,index,entry);
    FillEntries();
}

std::unique_ptr<EffAnalyzer> createEffAnalyzer(TTree* input,CutParams* effCut, const char* outTreeName )
{
    if (effCut->getIsMC())
        return std::unique_ptr<EffAnalyzer>(new EffAnalyzerMC(input,effCut,outTreeName));
    else
        return nullptr;
} 

template class EffAnalyzerBase<OniaMCData>;