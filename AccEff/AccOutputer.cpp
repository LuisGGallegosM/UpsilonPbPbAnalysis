
#include "AccOutputer.h"
#include "TCanvas.h"
#include "TPie.h"
#include <array>

AccOutputer::AccOutputer(const char* treeOutName) : TreeOutputer(treeOutName)
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
    etaVsPtQQCut =  createTH2("eta vs pt cut","p^{#mu#mu}_{t} vs |#eta^{#mu#mu}| cut");
    etaVsPtMu =     createTH2("eta vs pt singleMu","p^{#mu}_{t} vs |#eta^{#mu}|");
    etaVsPtMuCut =  createTH2("eta vs pt singleMu cut","p^{#mu}_{t} vs |#eta^{#mu}| Cut");

    ptHist=     createTH1("p_{t}","p^{#mu#mu}_{t}");
    ptHistCut=  createTH1("p_{t} cut","p^{#mu#mu}_{t} cut");
}

void AccOutputer::Write(const std::string& basename)
{
    TreeOutputer::Write();

    //calculate acceptancy
    ptAccHist = new  TEfficiency(*ptHistCut,*ptHist);
    ptAccHist->SetStatisticOption(TEfficiency::EStatOption::kFNormal);

    //write 2D plots
    writeToCanvas(etaVsPtQQ,    "|#eta^{#mu#mu}|","p^{#mu#mu}_{T} ( GeV/c )",basename+"_EtaPtQQ.pdf");
    writeToCanvas(etaVsPtMuCut, "|#eta^{#mu#mu}|","p^{#mu#mu}_{T} ( GeV/c )",basename+"_EtaPtQQCut.pdf");
    writeToCanvas(etaVsPtMu,    "|#eta^{#mu}|"   ,"p^{#mu}_{T} ( GeV/c )",basename+"_EtaPtMu.pdf");
    writeToCanvas(etaVsPtMuCut, "|#eta^{#mu}|"   ,"p^{#mu}_{T} ( GeV/c )",basename+"_EtaPtMuCut.pdf");

    //write 1D plots
    writeToCanvas(ptHist,       "p^{#mu#mu}_{T} ( GeV/c )", "N^{#mu#mu}",basename+"_Pt.pdf");
    writeToCanvas(ptHistCut,    "p^{#mu#mu}_{T} ( GeV/c )", "N^{#mu#mu}",basename+"_Pt.pdf");
    writeToCanvasEff(ptAccHist, "p^{#mu#mu}_{T} ( GeV/c )", "Acc",       basename+"_AccPt.pdf");
}

void AccOutputer::WriteData(const AccEffInput& dataIn,Int_t index, Long64_t entry,AccEffCutter& cut)
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
    if(cut.cut(&dataIn,index,entry))
    {
        ptHistCut->Fill(pT);
        etaVsPtQQCut->Fill(fabs(eta),pT);
        etaVsPtMuCut->Fill(fabs(mom4vecPl->Eta()),mom4vecPl->Pt());
        etaVsPtMuCut->Fill(fabs(mom4vecMi->Eta()),mom4vecMi->Pt());
        FillEntries();
    }
}

TH2F* AccOutputer::createTH2(const std::string& name,const std::string& title)
{
    const int binPtN = 40;
    const int binEtaN = 30;
    const float ptMax=8.0f;
    const float etaMax=3.0f;
    TH2F* result =new TH2F(name.data(),title.data(),binEtaN,0.0,etaMax,binPtN,0.0,ptMax);
    result->Sumw2();
    return result;
}

TH1F* AccOutputer::createTH1(const std::string& name,const std::string& title)
{
    static const std::array<float,7> pTBins  { 0.0f,2.0f,4.0f,6.0f,9.0f,12.0f,30.0f};
    TH1F* result=new TH1F(name.data(),title.data(),pTBins.size()-1,pTBins.data());
    result->Sumw2();
    return result;
}

void AccOutputer::writeToCanvas(TH1* hist,const std::string& xname,const std::string& yname, const std::string& outname)
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

void AccOutputer::writeToCanvasEff(TEfficiency* hist,const std::string& xname,const std::string& yname, const std::string& outname)
{
    TCanvas canvas("Fit_plot","fit",4,45,550,520);
    canvas.cd();
    TPad pad("pad","fit", 0.02, 0.02, 0.98, 0.98);
    pad.Draw();
    pad.cd();
    hist->Draw("COL");
    canvas.Write();
    canvas.SaveAs(outname.data());
}