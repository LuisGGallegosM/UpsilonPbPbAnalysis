
#include "AccEffOutputer.h"
#include "TCanvas.h"
#include "TPie.h"
#include <array>

AccEffOutputer::AccEffOutputer(const char* treeOutName,AccCutter* accCut, EffCutter* effCut) : 
    TreeOutputer(treeOutName), accCutter(accCut), effCutter(effCut)
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
    etaVsPtQQGen =      createTH2QQ("eta vs pt QQ Generated" ,"p^{#mu#mu}_{t} vs |#eta^{#mu#mu}|");
    etaVsPtQQDet =      createTH2QQ("eta vs pt QQ Detected"  ,"p^{#mu#mu}_{t} vs |#eta^{#mu#mu}| QQ Detected");
    etaVsPtQQRecoCut =  createTH2QQ("eta vs pt QQ Reco + Cut","p^{#mu#mu}_{t} vs |#eta^{#mu#mu}| QQ Reco + Cut");
    etaVsPtMuGen =      createTH2Mu("eta vs pt Mu Generated" ,"p^{#mu}_{t} vs |#eta^{#mu}| Mu Generated");
    etaVsPtMuDet =      createTH2Mu("eta vs pt Mu Detected"  ,"p^{#mu}_{t} vs |#eta^{#mu}| Mu Detected");
    etaVsPtMuRecoCut =  createTH2Mu("eta vs pt Mu Reco + Cut","p^{#mu}_{t} vs |#eta^{#mu}| Mu Reco + Cut");

    ptHistQQGen=    createTH1("p_{t} QQ Generated" ,"p^{#mu#mu}_{t} QQ Generated");
    ptHistQQDet=    createTH1("p_{t} QQ Detected"  ,"p^{#mu#mu}_{t} QQ Detected");
    ptHistQQRecoCut=createTH1("p_{t} QQ Reco + Cut","p^{#mu#mu}_{t} QQ Reco + Cut");
}

void AccEffOutputer::Write(const std::string& basename)
{
    TreeOutputer::Write();

    //calculate acceptancy
    ptQQAcceptancy = new  TEfficiency(*ptHistQQDet,*ptHistQQGen);
    ptQQEfficiency = new  TEfficiency(*ptHistQQRecoCut,*ptHistQQDet);
    ptQQAcceptancy->SetStatisticOption(TEfficiency::EStatOption::kFNormal);
    ptQQEfficiency->SetStatisticOption(TEfficiency::EStatOption::kFNormal);

    //write 2D plots
    writeToCanvas(etaVsPtQQGen,    "|y^{#mu#mu}|","p^{#mu#mu}_{T} ( GeV/c )",basename+"_EtaPtQQ_Gen.pdf");
    writeToCanvas(etaVsPtQQDet,    "|y^{#mu#mu}|","p^{#mu#mu}_{T} ( GeV/c )",basename+"_EtaPtQQ_Det.pdf");
    writeToCanvas(etaVsPtQQRecoCut,"|y^{#mu#mu}|","p^{#mu#mu}_{T} ( GeV/c )",basename+"_EtaPtQQ_RecoCut.pdf");
    writeToCanvas(etaVsPtMuGen,    "|#eta^{#mu}|"   ,"p^{#mu}_{T} ( GeV/c )",basename+"_EtaPtMu_Gen.pdf");
    writeToCanvas(etaVsPtMuDet,    "|#eta^{#mu}|"   ,"p^{#mu}_{T} ( GeV/c )",basename+"_EtaPtMu_Det.pdf");
    writeToCanvas(etaVsPtMuRecoCut,"|#eta^{#mu}|"   ,"p^{#mu}_{T} ( GeV/c )",basename+"_EtaPtMu_RecoCut.pdf");

    //write 1D plots
    writeToCanvas(ptHistQQGen,       "p^{#mu#mu}_{T} ( GeV/c )", "N_{Gen}^{#mu#mu}",basename+"_PtQQ_Gen.pdf");
    writeToCanvas(ptHistQQDet,       "p^{#mu#mu}_{T} ( GeV/c )", "N_{Det}^{#mu#mu}",basename+"_PtQQ_Det.pdf");
    writeToCanvas(ptHistQQRecoCut,   "p^{#mu#mu}_{T} ( GeV/c )", "N_{Rec}^{#mu#mu}",basename+"_PtQQ_RecoCut.pdf");
    writeToCanvasEff(ptQQAcceptancy, "p^{#mu#mu}_{T} ( GeV/c )", "Acc",    basename+"_PtQQ_Acceptancy.pdf");
    writeToCanvasEff(ptQQEfficiency, "p^{#mu#mu}_{T} ( GeV/c )", "Eff",    basename+"_PtQQ_Efficiency.pdf");
}

void AccEffOutputer::WriteData(const OniaInput& dataIn,Int_t index, Long64_t entry)
{
    //read variables
    TLorentzVector* mom4vec=(TLorentzVector*) dataIn.genQQ.mom4->At(index);

    int mupl_idx = dataIn.genQQ.mupl_idx[index];//plus muon index
    int mumi_idx = dataIn.genQQ.mumi_idx[index];//minus muon index
    TLorentzVector* mom4vecPl=(TLorentzVector*) dataIn.genMu.mom4->At(mupl_idx);
    TLorentzVector* mom4vecMi=(TLorentzVector*) dataIn.genMu.mom4->At(mumi_idx);

    mass= mom4vec->M();
    pT = mom4vec->Pt();
    y = fabs(mom4vec->Rapidity());
    phi = mom4vec->Phi();
    eta = mom4vec->Eta();
    Evt = entry;
    pdgId = dataIn.GenQQid[index];
    int AbsPdgId= abs(pdgId);

    //cuts
    if(y > 2.4f) return;
    float etaMuPl=fabs(mom4vecPl->Eta());
    float etaMuMi=fabs(mom4vecMi->Eta());
    float ptMuPl=mom4vecPl->Pt();
    float ptMuMi=mom4vecMi->Pt();

    //fill data hist for all onia and muons
    ptHistQQGen->Fill(pT);
    etaVsPtQQGen->Fill(y,pT);
    etaVsPtMuGen->Fill(etaMuPl,ptMuPl);
    etaVsPtMuGen->Fill(etaMuMi,ptMuMi);

    //fill data for onia with acceptancy cuts
    if(accCutter->cut(&dataIn,index,entry))
    {
        ptHistQQDet->Fill(pT);
        etaVsPtQQDet->Fill(y,pT);
        etaVsPtMuDet->Fill(etaMuPl,ptMuPl);
        etaVsPtMuDet->Fill(etaMuMi,ptMuMi);

        //fill data for onia with efficiency cuts
        if (effCutter->cut(&dataIn,index,entry))
        {
            ptHistQQRecoCut->Fill(pT);
            etaVsPtQQRecoCut->Fill(y,pT);
            etaVsPtMuRecoCut->Fill(etaMuPl,ptMuPl);
            etaVsPtMuRecoCut->Fill(etaMuMi,ptMuMi);
            FillEntries();
        }
    }
}

constexpr std::array<double,7> pTBins  { 0.0f,2.0f,4.0f,6.0f,9.0f,12.0f,30.0f};

TH2F* AccEffOutputer::createTH2QQ(const std::string& name,const std::string& title)
{
    const int binyN = 6;
    const float yMax = 3.0f;
    TH2F* result =new TH2F(name.data(),title.data(),binyN,0.0,yMax,pTBins.size()-1,pTBins.data());
    result->SetStats(false);
    result->Sumw2();
    return result;
}

TH2F* AccEffOutputer::createTH2Mu(const std::string& name,const std::string& title)
{
    const int binPtN = 40;
    const float ptMax = 20.0f;
    const int binEtaN = 30;
    const float etaMax = 3.0f;
    TH2F* result =new TH2F(name.data(),title.data(),binEtaN,0.0,etaMax,binPtN,0.0,ptMax);
    result->SetStats(false);
    result->Sumw2();
    return result;
}

TH1F* AccEffOutputer::createTH1(const std::string& name,const std::string& title)
{
    TH1F* result=new TH1F(name.data(),title.data(),pTBins.size()-1,pTBins.data());
    result->SetStats(false);
    result->Sumw2();
    return result;
}

void AccEffOutputer::writeToCanvasBase(TH1* hist,const std::string& xname,const std::string& yname, const std::string& outname, const std::string& option)
{
    TCanvas canvas("Fit_plot","fit",4,45,600,600);
    canvas.cd();
    TPad pad("pad","fit", 0.08, 0.08, 0.92, 0.92);
    pad.Draw();
    pad.cd();
    hist->GetYaxis()->SetTitle(yname.data());
    hist->GetXaxis()->SetTitle(xname.data());
    hist->Draw(option.data());
    canvas.Write();
    canvas.SaveAs(outname.data());
}

void AccEffOutputer::writeToCanvas(TH2* hist,const std::string& xname,const std::string& yname, const std::string& outname)
{
    writeToCanvasBase(hist,xname,yname,outname,"COLZ");
}

void AccEffOutputer::writeToCanvas(TH1* hist,const std::string& xname,const std::string& yname, const std::string& outname)
{
     writeToCanvasBase(hist,xname,yname,outname,"COL");
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