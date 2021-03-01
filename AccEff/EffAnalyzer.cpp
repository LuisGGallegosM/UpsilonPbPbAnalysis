#include "EffAnalyzer.h"
#include "AccEffAux.h"
#include "TCanvas.h"
#include "TPie.h"
#include <array>

EffAnalyzer::EffAnalyzer(OniaReader* input,EffCutter* effCut, OniaWriter* writer, TH1F* QQDetHist ) : 
    oniaWriter(writer),oniaReader(input),effCutter(effCut),ptHistQQDet(QQDetHist)
{
    if (writer->getType()!= QQtype::Reco)
        throw std::runtime_error("Error: Writer not set to Reco");
    //initialize Histograms
    etaVsPtQQRecoCut =  createTH2QQ("eta vs pt QQ Reco + Cut","p^{#mu#mu}_{t} vs |#eta^{#mu#mu}| QQ Reco + Cut");
    etaVsPtMuRecoCut =  createTH2Mu("eta vs pt Mu Reco + Cut","p^{#mu}_{t} vs |#eta^{#mu}| Mu Reco + Cut");
    ptHistQQRecoCut=    createTH1("p_{t} QQ Reco + Cut","p^{#mu#mu}_{t} QQ Reco + Cut");
}

void EffAnalyzer::ProcessEvent(Long64_t entry)
{
    Long64_t size=oniaReader->recoQQ_size;
    
    for(Long64_t i=0;i<size;++i)
    {
        Analyze(i,entry);
    }
}

void EffAnalyzer::Write(const std::string& basename)
{
    oniaWriter->Write();

    //write 2D plots
    writeToCanvas(etaVsPtQQRecoCut,"|y^{#mu#mu}|","p^{#mu#mu}_{T} ( GeV/c )",basename+"_EtaPtQQ_RecoCut.pdf");
    writeToCanvas(etaVsPtMuRecoCut,"|#eta^{#mu}|"   ,"p^{#mu}_{T} ( GeV/c )",basename+"_EtaPtMu_RecoCut.pdf");

    //write 1D plots
    writeToCanvas(ptHistQQRecoCut,   "p^{#mu#mu}_{T} ( GeV/c )", "N_{Rec}^{#mu#mu}",basename+"_PtQQ_RecoCut.pdf");

    //calculate efficiency
    ptQQEfficiency = new  TEfficiency(*ptHistQQRecoCut,*ptHistQQDet);
    ptQQEfficiency->SetStatisticOption(TEfficiency::EStatOption::kFNormal);
    writeToCanvasEff(ptQQEfficiency, "p^{#mu#mu}_{T} ( GeV/c )", "Eff",    basename+"_PtQQ_Efficiency.pdf");
}

void EffAnalyzer::Analyze(Int_t index, Long64_t entry)
{
    //fill data for onia with efficiency cuts
    if (effCutter->cut(oniaReader.get(),index,entry))
    {
        //read variables
        TLorentzVector* mom4vec=(TLorentzVector*) oniaReader->recoQQ_mom4->At(index);

        int mupl_idx = oniaReader->recoQQ_mupl_idx[index];//plus muon index
        int mumi_idx = oniaReader->recoQQ_mumi_idx[index];//minus muon index
        TLorentzVector* mom4vecPl=(TLorentzVector*) oniaReader->recoMu_mom4->At(mupl_idx);
        TLorentzVector* mom4vecMi=(TLorentzVector*) oniaReader->recoMu_mom4->At(mumi_idx);

        float pT = mom4vec->Pt();
        float y = fabs(mom4vec->Rapidity());
        float eta = mom4vec->Eta();

        float etaMuPl=fabs(mom4vecPl->Eta());
        float etaMuMi=fabs(mom4vecMi->Eta());
        float ptMuPl=mom4vecPl->Pt();
        float ptMuMi=mom4vecMi->Pt();

        ptHistQQRecoCut->Fill(pT);
        etaVsPtQQRecoCut->Fill(y,pT);
        etaVsPtMuRecoCut->Fill(etaMuPl,ptMuPl);
        etaVsPtMuRecoCut->Fill(etaMuMi,ptMuMi);
        oniaWriter->writeEntries(oniaReader.get(),index,entry);
    }
}