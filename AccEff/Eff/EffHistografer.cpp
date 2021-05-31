
#include "EffHistografer.h"

#include "../Common/AccEffAux.h"
#include "../../Drawing/Drawing.h"

EffHistografer::EffHistografer()
{
    //initialize Histograms
    etaVsPtQQRecoCut =  createTH2QQ("EtaPtQQ_RecoCut",    "p^{#mu#mu}_{t} vs |y^{#mu#mu}| QQ Reco + Cut");
    etaVsPtMuRecoCut =  createTH2Mu("EtaPtMu_RecoCut",  "p^{#mu}_{t} vs |#eta^{#mu}| Mu Reco + Cut");
    etaVsPtQQDet =      createTH2QQ("EtaPtQQ_Det",    "p^{#mu#mu}_{t} vs |y^{#mu#mu}| QQ Detectable");

    ptHistQQRecoCut=    createTH1(effNumName,"p^{#mu#mu}_{t} QQ Reco + Cut");
    ptHistQQDet=        createTH1(effDenName,"p^{#mu#mu}_{t} QQ Detectable");
}

void EffHistografer::finalCalculations(const std::string& basename)
{
    //write 2D plots
    writeToCanvas(etaVsPtQQRecoCut,"|y^{#mu#mu}|","p^{#mu#mu}_{T} ( GeV/c )",basename);
    writeToCanvas(etaVsPtQQDet,"|y^{#mu#mu}|","p^{#mu#mu}_{T} ( GeV/c )",basename);
    writeToCanvas(etaVsPtMuRecoCut,"|#eta^{#mu}|"   ,"p^{#mu}_{T} ( GeV/c )",basename);

    //write 1D plots
    writeToCanvas(ptHistQQDet,      "p^{#mu#mu}_{T} ( GeV/c )", "N_{Det}^{#mu#mu}",basename);
    writeToCanvas(ptHistQQRecoCut,  "p^{#mu#mu}_{T} ( GeV/c )", "N_{Rec}^{#mu#mu}",basename);

    //calculate efficiency
    ptQQEfficiency = createTEff(ptHistQQRecoCut,ptHistQQDet,effName,"Efficiency;p^{#mu#mu}_{T} ( GeV/c );#epsilon");

    etaVsPtQQEfficiency = createTEff(etaVsPtQQRecoCut,etaVsPtQQDet,"EtaPtQQ_Efficiency","Efficiency;|y^{#mu#mu}|;p^{#mu#mu}_{T} ( GeV/c )");

    writeToCanvasEff(ptQQEfficiency.get(), "p^{#mu#mu}_{T} ( GeV/c )", "#epsilon",    basename);

    writeToCanvasEff2D(etaVsPtQQEfficiency.get(),"|y^{#mu#mu}|", "p^{#mu#mu}_{T} ( GeV/c )",basename);
}

void EffHistografer::Write() const
{
    etaVsPtQQRecoCut->Write(0,TObject::kOverwrite);
    etaVsPtMuRecoCut->Write(0,TObject::kOverwrite);
    ptHistQQDet->Write(0,TObject::kOverwrite);
    ptHistQQRecoCut->Write(0,TObject::kOverwrite);
    ptQQEfficiency->Write(0,TObject::kOverwrite);
    etaVsPtQQEfficiency->Write(0,TObject::kOverwrite);
}

void EffHistografer::FillDet(double y, double pT, double weight)
{
    etaVsPtQQDet->Fill(y,pT,weight);
    ptHistQQDet->Fill(pT,weight*0.5);
}

void EffHistografer::FillRecoCut(const inputs* input, double weight)
{
    ptHistQQRecoCut->Fill(input->pT,weight*0.5);
    etaVsPtQQRecoCut->Fill(input->y,input->pT,weight);
    etaVsPtMuRecoCut->Fill(input->etaMuPl,input->ptMuPl,weight);
    etaVsPtMuRecoCut->Fill(input->etaMuMi,input->ptMuMi,weight);
}