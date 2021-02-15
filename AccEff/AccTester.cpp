#include "AccTester.h"
#include "TLorentzVector.h"
#include "TCanvas.h"
#include "TPie.h"

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
        if (cutter->cut(&dataIn,i,entry))
        {
            outputer->WriteData(dataIn,i,entry);
        }
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

    float pTBins[] = { 0.0f,2.0f,4.0f,6.0f,9.0f,12.0f,30.0f};
    float etaBins[] = { 0.0f,0.5f,1.0f,1.5f,2.0f,2.5f,3.0f,3.5,4.0,4.5f,5.0f };

    etaVsPtHist = new TH2F("eta vs pt","eta vs pt",6,pTBins,10,etaBins);
    ptHist= new TH1F("p_{t}","pt",6,pTBins);
    ptAccHist = new TH1F("Acc vs p_{t}","accPt",6,pTBins);
}

void writeToCanvas(TH1* hist,const char* xname,const char* yname, const char* outname)
{
    TCanvas canvas("Fit_plot","fit",4,45,550,520);
    canvas.cd();
    TPad pad("pad","fit", 0.02, 0.02, 0.98, 0.98);
    pad.Draw();
    pad.cd();
    hist->GetYaxis()->SetTitle(yname);
    hist->GetXaxis()->SetTitle(xname);
    hist->Draw("COL");
    canvas.Write();
    canvas.SaveAs(outname);
}

void writePie(const std::map<int,int>& pdgIds)
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
        pie.SetEntryLabel(i,Form("ID:%i",val.first));
        i++;
    }
    pie.SetTextSize(0.03f);
    pie.Draw("rsc");
    canvas.Write();
    canvas.SaveAs("pie.pdf");
}

void AccOutputer::Write()
{
    TreeOutputer::Write();
    writeToCanvas(etaVsPtHist,"p_{T} ( GeV/c )","|y^{#mu#mu}|","GraphEtaPt.pdf");
    writeToCanvas(ptHist,"p_{T} ( GeV/c )", "N","GraphPt.pdf");
    for(int i=0;i< ptHist->GetNbinsX();i++)
    {
        float ptBin = ptHist->GetBin(i);
        if (ptBin!=0.0f)
        {
            float acc= ptAccHist->GetBin(i) /ptBin;
            ptAccHist->SetBinContent(i,acc);
        }
        else
            ptAccHist->SetBinContent(i,0.0f);
    }
    writeToCanvas(ptAccHist,"p_{T} ( GeV/c )", "Acc","GraphPtAcc.pdf");
    writePie(pdgIds);
    
}

void AccOutputer::WriteData(const AccEffInput& dataIn,Int_t index, Long64_t entry)
{
    TLorentzVector* mom4vec=(TLorentzVector*) dataIn.mom4_GenQQ->At(index);

    mass= mom4vec->M();
    pT = mom4vec->Pt();
    y = mom4vec->Rapidity();
    phi = mom4vec->Phi();
    eta = mom4vec->Eta();
    Evt = entry;
    pdgId = dataIn.GenQQid[index];

    int AbsPdgId= abs(pdgId);
     if (pdgIds.find(AbsPdgId) != pdgIds.end())
        pdgIds.at(AbsPdgId) = pdgIds.at(AbsPdgId) +1;
     else
        pdgIds[AbsPdgId] = 1;

    etaVsPtHist->Fill(fabs(y),pT);
    ptHist->Fill(pT);
    FillEntries();
}