#include "AccTester.h"
#include "TLorentzVector.h"
#include "TCanvas.h"
#include "TPie.h"
#include <array>

AccTester::AccTester(TTree* treeIn, AccOutputer* outp , AccEffCutter* cut) 
: TreeProcessor(treeIn), cutter(cut), outputer(outp)
{
    TBranch* branch;

    //input branches
    addInput("Reco_QQ_4mom",&dataIn.mom4_RecoQQ);
    addInput("Reco_mu_4mom",&dataIn.mom4_RecoMu);
    addInput("Reco_QQ_size",&dataIn.recoQQsize);
    addInput("Reco_mu_size",&dataIn.recoMuSize);
    addInput("Reco_QQ_mupl_idx",dataIn.mupl_idx);
    addInput("Reco_QQ_mumi_idx",dataIn.mumi_idx);
    addInput("Reco_mu_SelectionType",dataIn.SelectionType);
    addInput("Reco_mu_nTrkWMea",dataIn.nTrkWMea);
    addInput("Reco_mu_nPixWMea",dataIn.nPixWMea);
    addInput("Reco_mu_dxy",dataIn.dxy);
    addInput("Reco_mu_dz",dataIn.dz);
    addInput("Reco_QQ_VtxProb",dataIn.VtxProb);
    addInput("Reco_QQ_trig",dataIn.trig);
    addInput("Reco_QQ_sign",dataIn.sign);
    addInput("Reco_mu_whichGen",dataIn.RecoMuWhichGen);
    addInput("Gen_QQ_4mom",&dataIn.mom4_GenQQ);
    addInput("Gen_mu_4mom",&dataIn.mom4_GenMu);
    addInput("Gen_QQ_size",&dataIn.genQQsize);
    addInput("Gen_mu_size",&dataIn.genMuSize);
    addInput("Gen_QQ_mupl_idx",dataIn.genQQ_mupl_idx);
    addInput("Gen_QQ_mumi_idx",dataIn.genQQ_mumi_idx);
    addInput("Gen_QQ_momId",dataIn.GenQQid);
    
    std::cout << "Using " << cutter->getName() <<" cutter.\n";
    return;
}

void AccTester::ProcessEvent(Long64_t entry)
{
    if (cutter->prescale(entry)) return;
    Long64_t size=dataIn.genQQsize;
    
    for(Long64_t i=0;i<size;++i)
    {
        outputer->WriteData(dataIn,i,entry,*cutter);
    }
}

AccOutputer::AccOutputer(const char* treeOutName) : TreeOutputer(treeOutName), pdgIds()
{
    addOutput("mass",&mass);
    addOutput("eta",&eta);
    addOutput("phi",&phi);
    addOutput("pT",&pT);
    addOutput("y",&y);
    addOutput("Evt",&Evt);
    addOutput("pdfId",&pdgId);

    std::array<float,7> pTBins  { 0.0f,2.0f,4.0f,6.0f,9.0f,12.0f,30.0f};
    const int binPtN = 40;
    const int binEtaN = 30;
    const float ptMax=8.0f;
    const float etaMax=3.0f;

    etaVsPtHist =       new TH2F("eta vs pt","p^{#mu#mu}_{t} vs |#eta^{#mu#mu}|",binEtaN,0.0,etaMax,binPtN,0.0,ptMax);
    etaVsPtCutHist =    new TH2F("eta vs pt cut","p^{#mu#mu}_{t} vs |#eta^{#mu#mu}| cut",binEtaN,0.0,etaMax,binPtN,0.0,ptMax);
    ptHist=             new TH1F("p_{t}","p^{#mu#mu}_{t}",pTBins.size()-1,pTBins.data());
    ptHistCut=          new TH1F("p_{t} cut","p^{#mu#mu}_{t} cut",pTBins.size()-1,pTBins.data());
    ptAccHist =         new TH1F("Acc vs p_{t}","Acc vs p^{#mu#mu}_{t}",pTBins.size()-1,pTBins.data());
    etaVsPtMuonHist =   new TH2F("eta vs pt singleMu","p^{#mu}_{t} vs |#eta^{#mu}|",binEtaN,0.0,etaMax,binPtN,0.0,ptMax);
    etaVsPtPrecutMuonHist =   new TH2F("eta vs pt singleMu pre","p^{#mu}_{t} vs |#eta^{#mu}|",binEtaN,0.0,etaMax,binPtN,0.0,ptMax);
    etaVsPtCutMuonHist =new TH2F("eta vs pt singleMu cut","p^{#mu}_{t} vs |#eta^{#mu}| Cut",binEtaN,0.0,etaMax,binPtN,0.0,ptMax);
}

void writeToCanvas(TH1* hist,const std::string& xname,const std::string& yname, const std::string& outname)
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

void writePie(const std::map<int,int>& pdgIds, const std::string& basename)
{
    TCanvas canvas("Fit_plot","fit",4,45,800,600);
    canvas.cd();
    TPad pad("pad","fit", 0.2, 0.2, 0.80, 0.80);
    pad.Draw();
    pad.cd();
    TPie pie("pie","pdgId",pdgIds.size());
    int i=0;
    for ( auto const& val : pdgIds)
    {
        pie.SetEntryVal(i,val.second);
        pie.SetEntryLabel(i,Form("  %i  ",val.first));
        i++;
    }
    pie.SetTextSize(0.03f);
    pie.Draw("rsc");
    canvas.Write();
    canvas.SaveAs((basename+"pie.pdf").data());
}

void AccOutputer::FillPtAccHist()
{
    int ptBinN =etaVsPtCutHist->GetNbinsY();
    
    for(int i=0;i< ptBinN;i++)
    {
        float acc= 0.0f;
        int pT=ptHist->GetBinContent(i);
        if (pT!=0) acc= ((float)ptHistCut->GetBinContent(i)) / pT;
        ptAccHist->SetBinContent(i,acc);
    }
}

void AccOutputer::Write(const std::string& basename)
{
    TreeOutputer::Write();
    FillPtAccHist();
    writeToCanvas(etaVsPtHist,"|#eta^{#mu#mu}|","p^{#mu#mu}_{T} ( GeV/c )",basename+"GraphEtaPt.pdf");
    writeToCanvas(etaVsPtCutHist,"|#eta^{#mu#mu}|","p^{#mu#mu}_{T} ( GeV/c )",basename+"GraphEtaPtCut.pdf");
    writeToCanvas(etaVsPtMuonHist,"|#eta^{#mu}|","p^{#mu}_{T} ( GeV/c )",basename+"GraphEtaPtMu.pdf");
    writeToCanvas(etaVsPtPrecutMuonHist,"|#eta^{#mu}|","p^{#mu}_{T} ( GeV/c )",basename+"GraphEtaPtMuPre.pdf");
    writeToCanvas(etaVsPtCutMuonHist,"|#eta^{#mu}|","p^{#mu}_{T} ( GeV/c )",basename+"GraphEtaPtMuCut.pdf");
    writeToCanvas(ptHist,"p^{#mu#mu}_{T} ( GeV/c )", "N",basename+"GraphPt.pdf");
    writeToCanvas(ptAccHist,"p^{#mu#mu}_{T} ( GeV/c )", "Acc",basename+"GraphPtAcc.pdf");
    writePie(pdgIds,basename);
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

    //classificate pdgIDS into a map
     if (pdgIds.find(AbsPdgId) != pdgIds.end())
        pdgIds.at(AbsPdgId) = pdgIds.at(AbsPdgId) +1;
     else
        pdgIds[AbsPdgId] = 1;

    //fill data hist for all onia
    etaVsPtHist->Fill(fabs(eta),pT);
    etaVsPtMuonHist->Fill(fabs(mom4vecPl->Eta()),mom4vecPl->Pt());
    etaVsPtMuonHist->Fill(fabs(mom4vecMi->Eta()),mom4vecMi->Pt());
    ptHist->Fill(pT);
    if (cut.isMuonInAcceptance(mom4vecPl)) etaVsPtPrecutMuonHist->Fill(fabs(mom4vecPl->Eta()),mom4vecPl->Pt());
    if (cut.isMuonInAcceptance(mom4vecMi)) etaVsPtPrecutMuonHist->Fill(fabs(mom4vecMi->Eta()),mom4vecMi->Pt());

    //fill data for onia with cuts
    if(cut.cut(&dataIn,index,entry))
    {
        ptHistCut->Fill(pT);
        etaVsPtCutHist->Fill(fabs(eta),pT);
        etaVsPtCutMuonHist->Fill(fabs(mom4vecPl->Eta()),mom4vecPl->Pt());
        etaVsPtCutMuonHist->Fill(fabs(mom4vecMi->Eta()),mom4vecMi->Pt());
        FillEntries();
    }
}