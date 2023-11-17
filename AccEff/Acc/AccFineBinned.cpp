
#include"AccFineBinned.h"

#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TFile.h>

#include "../Common/AccEffAux.h"

#include"TLorentzVector.h"

#include<iostream>

using namespace std;


void AccFineBinning(const char* filename, const char* outfilename)
{
    AcceptanceFinebinned finebin;
    finebin.calculate(filename,outfilename);
}

//Defining program
void AcceptanceFinebinned::calculate(const char* filename, const char* outfilename)
{
  
  //Defining pt binning
  Float_t PtBins[29] = {0.0,0.5,1.0,1.5,2.0,2.5,3.0,3.5,4.0,4.5,5.0,5.5,6.0,6.75,7.5,8.25,9.0,9.75,10.5,11.25,12.0,16.5,21.0,25.5,30.0,35.0,40.0,45.0,50.0};
  Int_t PtnBins= 28;

  //Defining rapidity binning
  Float_t RapBins[25] = {-2.4,-2.2,-2.0,-1.8,-1.6,-1.4,-1.2,-1.0,-0.8,-0.6,-0.4,-0.2,0.0,0.2,0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,2.0,2.2,2.4};
  Int_t RapnBins = 24;

  //Defining Acceptance Denominator Histogram
   TH2F *hGenUpsilonAccDen = new TH2F ("hGenUpsilonAccDen", "2D Acceptance Denominator", PtnBins, PtBins, RapnBins, RapBins);
   //Defining Acceptance Numerator Histogram
   TH2F *hGenUpsilonAccNum = new TH2F ("hGenUpsilonAccNum", "2D Acceptance Numerator", PtnBins, PtBins, RapnBins, RapBins);
   //Defining Weighted Acceptance Denominator Histogram
   TH2F *hGenUpsilonAccDenW = new TH2F ("hGenUpsilonAccDenW", "2D Acceptance Weighted Denominator", PtnBins, PtBins, RapnBins, RapBins);
   //Defining Weighted Acceptance Numerator Histogram
   TH2F *hGenUpsilonAccNumW = new TH2F ("hGenUpsilonAccNumW", "2D Acceptance Weighted Numerator", PtnBins, PtBins, RapnBins, RapBins);
   
    TFile *File = TFile::Open(filename);
    TTree *MyTree = (TTree*)File->Get("hionia/myTree");
    int nevents= (int)MyTree->GetEntries();
    cout<<"nevents = "<<nevents<<"\n";

    Long64_t nentries = nevents;

  TBranch *b_Gen_QQ_size = MyTree->GetBranch("Gen_QQ_size");
  b_Gen_QQ_size->SetAddress(&Gen_QQ_size);

  //4-moms
  TBranch *b_Gen_QQ_4mom;
  TClonesArray *Gen_QQ_4mom = new TClonesArray();
  MyTree->SetBranchAddress("Gen_QQ_4mom", &Gen_QQ_4mom, &b_Gen_QQ_4mom);
   
  TBranch *b_Gen_mu_4mom;
  TClonesArray *Gen_mu_4mom = new TClonesArray();
  MyTree->SetBranchAddress("Gen_mu_4mom", &Gen_mu_4mom, &b_Gen_mu_4mom);

  TBranch *b_Gen_QQ_mupl_4mom;
  TClonesArray *Gen_QQ_mupl_4mom = new TClonesArray();
  MyTree->SetBranchAddress("Gen_QQ_mupl_4mom", &Gen_QQ_mupl_4mom, &b_Gen_QQ_mupl_4mom);

  TBranch *b_Gen_QQ_mumi_4mom;
  TClonesArray *Gen_QQ_mumi_4mom = new TClonesArray();
  MyTree->SetBranchAddress("Gen_QQ_mumi_4mom", &Gen_QQ_mumi_4mom, &b_Gen_QQ_mumi_4mom);

  for (Long64_t jentry=0; jentry<nentries;jentry++)//Loop on ALL events
    //for (Long64_t jentry=0; jentry<10000;jentry++)//Loop on first 10000 events
    {
      
      if( (jentry % 100000)==0 ) cout<<"Reading event "<<jentry<<"\n";
            Gen_QQ_4mom->Clear();
      Gen_mu_4mom->Clear();

      b_Gen_QQ_size->GetEntry(jentry);
      if(Gen_QQ_size > 0)
      {
        b_Gen_QQ_4mom->GetEntry(jentry);
        b_Gen_mu_4mom->GetEntry(jentry);
        b_Gen_QQ_mupl_4mom->GetEntry(jentry);
        b_Gen_QQ_mumi_4mom->GetEntry(jentry);
      }
    
      for(int iQQ=0; iQQ < Gen_QQ_size; iQQ++)//Loop on generated upsilon
	{
	  TLorentzVector *GenQQ = (TLorentzVector*) Gen_QQ_4mom->At(iQQ);
	  float UpsilonPt = GenQQ->Pt();
	  float UpsilonEta = GenQQ->Eta();
	  float UpsilonRapidity = GenQQ->Rapidity();

	  TLorentzVector *GenMuPl = (TLorentzVector*) Gen_QQ_mupl_4mom->At(iQQ);
	  float MuPlPt = GenMuPl->Pt();
	  float MuPlEta = GenMuPl->Eta();

	  TLorentzVector *GenMuMi = (TLorentzVector*) Gen_QQ_mumi_4mom->At(iQQ);
	  float MuMiPt = GenMuMi->Pt();
	  float MuMiEta = GenMuMi->Eta();

	  //Defining Acceptance conditions as booleans
	  bool UpsilonRapMax = abs(UpsilonRapidity) < 2.4;
       
          bool MuPlPtMin = MuPlPt > 3.5;
	  bool MuPlEtaMax = abs(MuPlEta) < 2.4;
	   
          bool MuMiPtMin = MuMiPt > 3.5;
	  bool MuMiEtaMax = abs(MuMiEta) < 2.4;

	  bool MuonPtMin = (MuPlPtMin && MuMiPtMin);
	  bool MuonEtaMax = (MuPlEtaMax && MuMiEtaMax);

	  //Get weight value using weight function
	  Double_t WeightVal = weightF(UpsilonPt);
	  
	  if(WeightVal==0) break;

	  if( abs(UpsilonRapidity) < 2.4) {
	    //Check for conditions and fill denominator histograms
	    hGenUpsilonAccDen->Fill(UpsilonPt,UpsilonRapidity);
	    hGenUpsilonAccDenW->Fill(UpsilonPt,UpsilonRapidity,(1/WeightVal));

	    if( ((MuPlPtMin==true) & (MuMiPtMin==true)) & ((MuPlEtaMax==true) & (MuMiEtaMax==true)) ){
	      //Check for conditions and fill numerator histograms
	      hGenUpsilonAccNum->Fill(UpsilonPt,UpsilonRapidity);
	      hGenUpsilonAccNumW->Fill(UpsilonPt,UpsilonRapidity,(1/WeightVal));
	    }
	  }
	}
    }


  //Calculate Acceptance by dividing histograms
  TH2F *hGenUpsilonAcc  = new TH2F(*hGenUpsilonAccNum); hGenUpsilonAcc->Divide(hGenUpsilonAccDen);
  TH2F *hGenUpsilonAccW  = new TH2F(*hGenUpsilonAccNumW); hGenUpsilonAccW->Divide(hGenUpsilonAccDenW);

  //Calculate weighted to unweighted ratio
  TH2F *hGenUpsilonAccWtoUWratio  = new TH2F(*hGenUpsilonAccW); hGenUpsilonAccWtoUWratio->Divide(hGenUpsilonAcc);


  //Set histograms parameters and write them out on root file

  TFile *fOut = new TFile(outfilename,"RECREATE");

   hGenUpsilonAcc->SetStats(0);
   hGenUpsilonAcc->SetContour(1000);
   hGenUpsilonAcc->SetTitle("Acceptance Upsilon pT vs y distribution");
   hGenUpsilonAcc->SetTitleSize(1);
   hGenUpsilonAcc->SetName("hGenUpsilonPtvsRapSlide5");
   hGenUpsilonAcc->GetXaxis()->SetTitleSize(0.06);
   hGenUpsilonAcc->GetXaxis()->SetTitleOffset(0.65);
   hGenUpsilonAcc->GetXaxis()->SetLabelSize(0.04);
   hGenUpsilonAcc->GetXaxis()->SetTitle("Upsilon pT (GeV)");
   hGenUpsilonAcc->GetYaxis()->SetTitle("Upsilon y");
   hGenUpsilonAcc->GetYaxis()->SetTitleSize(0.06);
   hGenUpsilonAcc->GetYaxis()->SetTitleOffset(0.65);
   hGenUpsilonAcc->GetYaxis()->SetLabelSize(0.04);
   hGenUpsilonAcc->SetOption("colz");
   
   hGenUpsilonAcc->Write();
   hGenUpsilonAccDen->Write();
   hGenUpsilonAccNum->Write();

   hGenUpsilonAccW->SetStats(0);
   hGenUpsilonAccW->SetContour(1000);
   hGenUpsilonAccW->SetTitle("Weighted Acceptance Upsilon pT vs y distribution");
   hGenUpsilonAccW->SetTitleSize(1);
   hGenUpsilonAccW->SetName("hGenUpsilonAccW");
   hGenUpsilonAccW->GetXaxis()->SetTitleSize(0.06);
   hGenUpsilonAccW->GetXaxis()->SetTitleOffset(0.65);
   hGenUpsilonAccW->GetXaxis()->SetLabelSize(0.04);
   hGenUpsilonAccW->GetXaxis()->SetTitle("Upsilon pT (GeV)");
   hGenUpsilonAccW->GetYaxis()->SetTitle("Upsilon y");
   hGenUpsilonAccW->GetYaxis()->SetTitleSize(0.06);
   hGenUpsilonAccW->GetYaxis()->SetTitleOffset(0.65);
   hGenUpsilonAccW->GetYaxis()->SetLabelSize(0.04);
   hGenUpsilonAccW->SetOption("colz");
   
   hGenUpsilonAccW->Write();
   hGenUpsilonAccDenW->Write();
   hGenUpsilonAccNumW->Write();

   hGenUpsilonAccWtoUWratio->SetStats(0);
   hGenUpsilonAccWtoUWratio->SetContour(1000);
   hGenUpsilonAccWtoUWratio->SetTitle("2D Acceptance Weighted to Unweighted Ratio");
   hGenUpsilonAccWtoUWratio->GetXaxis()->SetTitleSize(0.06);
   hGenUpsilonAccWtoUWratio->GetXaxis()->SetTitleOffset(0.65);
   hGenUpsilonAccWtoUWratio->GetXaxis()->SetLabelSize(0.04);
   hGenUpsilonAccWtoUWratio->SetName("hGenUpsilonAccWtoUWratio");
   hGenUpsilonAccWtoUWratio->GetXaxis()->SetTitle("Upsilon pT (GeV)");
   hGenUpsilonAccWtoUWratio->GetYaxis()->SetTitle("Upsilon y");
   hGenUpsilonAccWtoUWratio->GetYaxis()->SetTitleSize(0.06);
   hGenUpsilonAccWtoUWratio->GetYaxis()->SetTitleOffset(0.65);
   hGenUpsilonAccWtoUWratio->GetYaxis()->SetLabelSize(0.04);
   hGenUpsilonAccWtoUWratio->SetOption("colz");
   
   hGenUpsilonAccWtoUWratio->Write();
   
   fOut->Close();
}