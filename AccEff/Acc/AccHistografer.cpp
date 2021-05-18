
#include"AccHistografer.h"

#include "../Common/AccEffAux.h"
#include "../../Drawing/Drawing.h"

//AccHistografer

AccHistografer::AccHistografer()
{
    //initialize Histograms
    etaVsPtQQGen = createTH2QQ("EtaPtQQ_Gen",  "p^{#mu#mu}_{t} vs |y^{#mu#mu}|");
    etaVsPtQQDet = createTH2QQ("EtaPtQQ_Det", "p^{#mu#mu}_{t} vs |y^{#mu#mu}| QQ Detected");

    etaVsPtMuGen = createTH2Mu("EtaPtMu_Gen","p^{#mu}_{t} vs |#eta^{#mu}| Mu Generated");
    etaVsPtMuDet = createTH2Mu("EtaPtMu_Det","p^{#mu}_{t} vs |#eta^{#mu}| Mu Detected");

    ptHistQQGen= createTH1(accDenName ,"p^{#mu#mu}_{t} QQ Generated");
    ptHistQQDet= createTH1(accNumName ,"p^{#mu#mu}_{t} QQ Detectable");
}

void AccHistografer::FillGen(const inputs* in,float weight)
{
    ptHistQQGen->Fill(in->pT,weight);
    etaVsPtQQGen->Fill(in->y,in->pT,weight);
    etaVsPtMuGen->Fill(in->etaMuPl,in->ptMuPl,weight);
    etaVsPtMuGen->Fill(in->etaMuMi,in->ptMuMi,weight);
}

void AccHistografer::FillDet(const inputs* in,float weight)
{
    ptHistQQDet->Fill(in->pT,weight);
    etaVsPtQQDet->Fill(in->y,in->pT,weight);
    etaVsPtMuDet->Fill(in->etaMuPl,in->ptMuPl,weight);
    etaVsPtMuDet->Fill(in->etaMuMi,in->ptMuMi,weight);
}

void AccHistografer::finalCalculations(const std::string& basename)
{
    //calculate acceptancy
    ptQQAcceptancy=createTEff(ptHistQQDet,ptHistQQGen,accName, "Acceptance;p^{#mu#mu}_{T} ( GeV/c );#alpha");
    etaVsPtQQAcceptancy=createTEff(etaVsPtQQDet,etaVsPtQQGen,"EtaPtQQ_Acceptancy","Acceptance;|y^{#mu#mu}|;p^{#mu#mu}_{T} ( GeV/c )");

    //write 2D plots
    writeToCanvas(etaVsPtQQGen,    "|y^{#mu#mu}|","p^{#mu#mu}_{T} ( GeV/c )",basename);
    writeToCanvas(etaVsPtQQDet,    "|y^{#mu#mu}|","p^{#mu#mu}_{T} ( GeV/c )",basename);
    writeToCanvas(etaVsPtMuGen,    "|#eta^{#mu}|"   ,"p^{#mu}_{T} ( GeV/c )",basename);
    writeToCanvas(etaVsPtMuDet,    "|#eta^{#mu}|"   ,"p^{#mu}_{T} ( GeV/c )",basename);

    //write 1D plots
    writeToCanvas(ptHistQQGen,       "p^{#mu#mu}_{T} ( GeV/c )", "N_{Gen}^{#mu#mu}",basename);
    writeToCanvas(ptHistQQDet,       "p^{#mu#mu}_{T} ( GeV/c )", "N_{Det}^{#mu#mu}",basename);
    writeToCanvasEff(ptQQAcceptancy.get(), "p^{#mu#mu}_{T} ( GeV/c )", "#alpha",    basename);

    writeToCanvasEff2D(etaVsPtQQAcceptancy.get(), "|y^{#mu#mu}|" , "p^{#mu#mu}_{T} ( GeV/c )",  basename);
}

void AccHistografer::Write() const
{
    etaVsPtQQGen->Write(0,TObject::kOverwrite);
    etaVsPtQQDet->Write(0,TObject::kOverwrite);
    etaVsPtMuGen->Write(0,TObject::kOverwrite);
    etaVsPtMuDet->Write(0,TObject::kOverwrite);

    ptHistQQGen->Write(0,TObject::kOverwrite);
    ptHistQQDet->Write(0,TObject::kOverwrite);
    ptQQAcceptancy->Write(0,TObject::kOverwrite);
    etaVsPtQQAcceptancy->Write(0,TObject::kOverwrite);
}