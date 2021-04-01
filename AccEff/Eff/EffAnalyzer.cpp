
#include "EffAnalyzer.h"

#include <array>

#include "TCanvas.h"
#include "TPie.h"

#include "../Common/AccEffAux.h"

std::unique_ptr<EffAnalyzer> createEffAnalyzer(TTree* input,CutParams* effCut, const char* outTreeName )
{
    if (effCut->getIsMC())
        return std::unique_ptr<EffAnalyzer>(new EffAnalyzerMC(input,effCut,outTreeName));
    else
        return nullptr;
}

EffHistografer::EffHistografer()
{
    //initialize Histograms
    etaVsPtQQRecoCut =  createTH2QQ("y vs pt QQ Reco + Cut",    "p^{#mu#mu}_{t} vs |y^{#mu#mu}| QQ Reco + Cut");
    etaVsPtMuRecoCut =  createTH2Mu("eta vs pt Mu Reco + Cut",  "p^{#mu}_{t} vs |#eta^{#mu}| Mu Reco + Cut");
    etaVsPtQQDet =      createTH2QQ("y vs pt QQ Detectable",    "p^{#mu#mu}_{t} vs |y^{#mu#mu}| QQ Detectable");

    ptHistQQRecoCut=    createTH1(effNumName,"p^{#mu#mu}_{t} QQ Reco + Cut");
    ptHistQQDet=        createTH1(effDenName,"p^{#mu#mu}_{t} QQ Detectable");
}

void EffHistografer::Write(const std::string& basename)
{
    //write 2D plots
    writeToCanvas(etaVsPtQQRecoCut,"|y^{#mu#mu}|","p^{#mu#mu}_{T} ( GeV/c )",basename+"_EtaPtQQ_RecoCut.pdf");
    writeToCanvas(etaVsPtQQDet,"|y^{#mu#mu}|","p^{#mu#mu}_{T} ( GeV/c )",basename+"_EtaPtQQ_Det.pdf");
    writeToCanvas(etaVsPtMuRecoCut,"|#eta^{#mu}|"   ,"p^{#mu}_{T} ( GeV/c )",basename+"_EtaPtMu_RecoCut.pdf");

    //write 1D plots
    writeToCanvas(ptHistQQDet,      "p^{#mu#mu}_{T} ( GeV/c )", "N_{Det}^{#mu#mu}",basename+"_PtQQ_Det.pdf");
    writeToCanvas(ptHistQQRecoCut,  "p^{#mu#mu}_{T} ( GeV/c )", "N_{Rec}^{#mu#mu}",basename+"_PtQQ_RecoCut.pdf");

    etaVsPtQQRecoCut->Write(0,TObject::kOverwrite);
    etaVsPtMuRecoCut->Write(0,TObject::kOverwrite);
    ptHistQQDet->Write(0,TObject::kOverwrite);
    ptHistQQRecoCut->Write(0,TObject::kOverwrite);

    //calculate efficiency
    ptQQEfficiency = createTEff(ptHistQQRecoCut,ptHistQQDet,"pt QQ Efficiency","Efficiency;p^{#mu#mu}_{T} ( GeV/c );#epsilon");

    etaVsPtQQEfficiency = createTEff(etaVsPtQQRecoCut,etaVsPtQQDet,"eta vs pt QQ Efficiency","Efficiency;|y^{#mu#mu}|;p^{#mu#mu}_{T} ( GeV/c )");

    writeToCanvasEff(ptQQEfficiency.get(), "p^{#mu#mu}_{T} ( GeV/c )", "#epsilon",    basename+"_PtQQ_Efficiency.pdf");

    writeToCanvasEff2D(etaVsPtQQEfficiency.get(),"|y^{#mu#mu}|", "p^{#mu#mu}_{T} ( GeV/c )",basename+"_EtaPtQQ_Efficiency.pdf");

    ptQQEfficiency->Write(0,TObject::kOverwrite);
    etaVsPtQQEfficiency->Write(0,TObject::kOverwrite);
}

void EffHistografer::FillDet(float y, float pT)
{
    etaVsPtQQDet->Fill(y,pT);
    ptHistQQDet->Fill(pT);
}

void EffHistografer::FillRecoCut(const inputs* input)
{
    ptHistQQRecoCut->Fill(input->pT);
    etaVsPtQQRecoCut->Fill(input->y,input->pT);
    etaVsPtMuRecoCut->Fill(input->etaMuPl,input->ptMuPl);
    etaVsPtMuRecoCut->Fill(input->etaMuMi,input->ptMuMi);
}