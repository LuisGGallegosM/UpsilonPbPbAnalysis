
#include "AccEffOutputer.h"
#include "TCanvas.h"
#include "TPie.h"
#include <array>

AccEffOutputer::AccEffOutputer(const char* treeOutName) : TreeOutputer(treeOutName)
{
    //variables to read from tree
    addOutput("mass",&mass);
    addOutput("eta",&eta);
    addOutput("phi",&phi);
    addOutput("pT",&pT);
    addOutput("y",&y);
    addOutput("Evt",&Evt);
    addOutput("pdfId",&pdgId);

    //initialize Histograms
    etaVsPtQQ =     createTH2("eta vs pt","p^{#mu#mu}_{t} vs |#eta^{#mu#mu}|");
    etaVsPtQQAcc =  createTH2("eta vs pt acc","p^{#mu#mu}_{t} vs |#eta^{#mu#mu}| Acc");
    etaVsPtQQEff =  createTH2("eta vs pt eff","p^{#mu#mu}_{t} vs |#eta^{#mu#mu}| Eff");
    etaVsPtMu =     createTH2("eta vs pt singleMu","p^{#mu}_{t} vs |#eta^{#mu}|");
    etaVsPtMuAcc =  createTH2("eta vs pt singleMu Acc","p^{#mu}_{t} vs |#eta^{#mu}| Acc");
    etaVsPtMuEff =  createTH2("eta vs pt singleMu Acc","p^{#mu}_{t} vs |#eta^{#mu}| Eff");

    ptHist=     createTH1("p_{t}","p^{#mu#mu}_{t}");
    ptHistAcc=  createTH1("p_{t} Acc","p^{#mu#mu}_{t} Acc");
    ptHistEff=  createTH1("p_{t} Eff","p^{#mu#mu}_{t} Eff");
}

void AccEffOutputer::Write(const std::string& basename)
{
    TreeOutputer::Write();

    //calculate acceptancy
    ptAcceptancy = new  TEfficiency(*ptHistAcc,*ptHist);
    ptEfficiency = new  TEfficiency(*ptHistEff,*ptHistAcc);
    ptAcceptancy->SetStatisticOption(TEfficiency::EStatOption::kFNormal);
    ptEfficiency->SetStatisticOption(TEfficiency::EStatOption::kFNormal);

    //write 2D plots
    writeToCanvas(etaVsPtQQ,    "|#eta^{#mu#mu}|","p^{#mu#mu}_{T} ( GeV/c )",basename+"_EtaPtQQ.pdf");
    writeToCanvas(etaVsPtQQAcc, "|#eta^{#mu#mu}|","p^{#mu#mu}_{T} ( GeV/c )",basename+"_EtaPtQQAcc.pdf");
    writeToCanvas(etaVsPtQQEff, "|#eta^{#mu#mu}|","p^{#mu#mu}_{T} ( GeV/c )",basename+"_EtaPtQQEff.pdf");
    writeToCanvas(etaVsPtMu,    "|#eta^{#mu}|"   ,"p^{#mu}_{T} ( GeV/c )",basename+"_EtaPtMu.pdf");
    writeToCanvas(etaVsPtMuAcc, "|#eta^{#mu}|"   ,"p^{#mu}_{T} ( GeV/c )",basename+"_EtaPtMuAcc.pdf");
    writeToCanvas(etaVsPtMuEff, "|#eta^{#mu}|"   ,"p^{#mu}_{T} ( GeV/c )",basename+"_EtaPtMuEff.pdf");

    //write 1D plots
    writeToCanvas(ptHist,       "p^{#mu#mu}_{T} ( GeV/c )", "N^{#mu#mu}",basename+"_Pt.pdf");
    writeToCanvas(ptHistAcc,    "p^{#mu#mu}_{T} ( GeV/c )", "N^{#mu#mu}",basename+"_PtAcc.pdf");
    writeToCanvas(ptHistEff,    "p^{#mu#mu}_{T} ( GeV/c )", "N^{#mu#mu}",basename+"_PtEff.pdf");
    writeToCanvasEff(ptAcceptancy, "p^{#mu#mu}_{T} ( GeV/c )", "Acc",    basename+"_Acceptancy.pdf");
    writeToCanvasEff(ptEfficiency, "p^{#mu#mu}_{T} ( GeV/c )", "Eff",    basename+"_Efficiency.pdf");
}

void AccEffOutputer::WriteData(const OniaInput& dataIn,Int_t index, Long64_t entry,AccCutter& accCut,EffCutter& effCut)
{
    //read variables
    TLorentzVector* mom4vec=(TLorentzVector*) dataIn.mom4_GenQQ->At(index);

    int mupl_idx = dataIn.genQQ_mupl_idx[index];//plus muon index
    int mumi_idx = dataIn.genQQ_mumi_idx[index];//minus muon index
    TLorentzVector* mom4vecPl=(TLorentzVector*) dataIn.mom4_GenMu->At(mupl_idx);
    TLorentzVector* mom4vecMi=(TLorentzVector*) dataIn.mom4_GenMu->At(mumi_idx);

    mass= mom4vec->M();
    pT = mom4vec->Pt();
    y = mom4vec->Rapidity();
    phi = mom4vec->Phi();
    eta = mom4vec->Eta();
    Evt = entry;
    pdgId = dataIn.GenQQid[index];
    int AbsPdgId= abs(pdgId);

    //cuts
    if((pT<3.5f) || (pT>50.0f)) return;
    if(fabs(y) > 2.4f) return;

    //fill data hist for all onia and muons (denominator)
    etaVsPtQQ->Fill(fabs(eta),pT);
    etaVsPtMu->Fill(fabs(mom4vecPl->Eta()),mom4vecPl->Pt());
    etaVsPtMu->Fill(fabs(mom4vecMi->Eta()),mom4vecMi->Pt());
    ptHist->Fill(pT);

    //fill data for onia with acceptancy cuts (numerator)
    if(accCut.cut(&dataIn,index,entry))
    {
        ptHistAcc->Fill(pT);
        etaVsPtQQAcc->Fill(fabs(eta),pT);
        etaVsPtMuAcc->Fill(fabs(mom4vecPl->Eta()),mom4vecPl->Pt());
        etaVsPtMuAcc->Fill(fabs(mom4vecMi->Eta()),mom4vecMi->Pt());

        if (effCut.cut(&dataIn,index,entry))
        {
            ptHistEff->Fill(pT);
            etaVsPtQQEff->Fill(fabs(eta),pT);
            etaVsPtMuEff->Fill(fabs(mom4vecPl->Eta()),mom4vecPl->Pt());
            etaVsPtMuEff->Fill(fabs(mom4vecMi->Eta()),mom4vecMi->Pt());
        }
        FillEntries();
    }
}

TH2F* AccEffOutputer::createTH2(const std::string& name,const std::string& title)
{
    const int binPtN = 40;
    const int binEtaN = 30;
    const float ptMax=8.0f;
    const float etaMax=3.0f;
    TH2F* result =new TH2F(name.data(),title.data(),binEtaN,0.0,etaMax,binPtN,0.0,ptMax);
    result->Sumw2();
    return result;
}

TH1F* AccEffOutputer::createTH1(const std::string& name,const std::string& title)
{
    static const std::array<float,7> pTBins  { 0.0f,2.0f,4.0f,6.0f,9.0f,12.0f,30.0f};
    TH1F* result=new TH1F(name.data(),title.data(),pTBins.size()-1,pTBins.data());
    result->Sumw2();
    return result;
}

void AccEffOutputer::writeToCanvas(TH1* hist,const std::string& xname,const std::string& yname, const std::string& outname)
{
    TCanvas canvas("Fit_plot","fit",4,45,550,520);
    canvas.cd();
    TPad pad("pad","fit", 0.02, 0.02, 0.98, 0.98);
    pad.Draw();
    pad.cd();
    hist->GetYaxis()->SetTitle(yname.data());
    hist->GetXaxis()->SetTitle(xname.data());
    hist->Draw("COL");
    canvas.Write();
    canvas.SaveAs(outname.data());
}

void AccEffOutputer::writeToCanvasEff(TEfficiency* hist,const std::string& xname,const std::string& yname, const std::string& outname)
{
    TCanvas canvas("Fit_plot","fit",4,45,550,520);
    canvas.cd();
    TPad pad("pad","fit", 0.02, 0.02, 0.98, 0.98);
    pad.Draw();
    pad.cd();
    hist->Draw();
    canvas.Write();
    canvas.SaveAs(outname.data());
}