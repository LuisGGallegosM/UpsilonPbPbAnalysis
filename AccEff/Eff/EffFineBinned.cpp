
#include"EffFineBinned.h"

#include"TLorentzVector.h"
#include"TH2.h"
#include"TFile.h"
#include"TChain.h"

#include "../Common/AccEffAux.h"

#include<iostream>

using namespace std;

void EffFineBinning(const char* filename, const char* outfilename)
{
    EfficiencyFineBinned finebin;
    finebin.calculate(filename,outfilename);
}

void EfficiencyFineBinned::calculate(const char* filename, const char* outfilename)
{
  
  //Defining pt binning
  Float_t PtBins[29] = {0.0,0.5,1.0,1.5,2.0,2.5,3.0,3.5,4.0,4.5,5.0,5.5,6.0,6.75,7.5,8.25,9.0,9.75,10.5,11.25,12.0,16.5,21.0,25.5,30.0,35.0,40.0,45.0,50.0};
  Int_t PtnBins= 28;

  //Defining rapidity binning
  Float_t RapBins[25] = {-2.4,-2.2,-2.0,-1.8,-1.6,-1.4,-1.2,-1.0,-0.8,-0.6,-0.4,-0.2,0.0,0.2,0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,2.0,2.2,2.4};
  Int_t RapnBins = 24;

  //Defining Efficiency Denominator Histogram
   TH2F *hGenUpsilonEffDen = new TH2F ("hGenUpsilonEffDen", "2D Effeptance Denominator", PtnBins, PtBins, RapnBins, RapBins);
   //Defining Efficiency Numerator Histogram
   TH2F *hGenUpsilonEffNum = new TH2F ("hGenUpsilonEffNum", "2D Efficiency Numerator", PtnBins, PtBins, RapnBins, RapBins);
   //Defining Weighted Efficiency Denominator Histogram
   TH2F *hGenUpsilonEffDenW = new TH2F ("hGenUpsilonEffDenW", "2D Efficiency Weighted Denominator", PtnBins, PtBins, RapnBins, RapBins);
   //Defining Weighted Effificency Numerator Histogram
   TH2F *hGenUpsilonEffNumW = new TH2F ("hGenUpsilonEffNumW", "2D Efficiency Weighted Numerator", PtnBins, PtBins, RapnBins, RapBins);

  //Declaring Tree branches
  TFile *File = TFile::Open(filename);
  TTree *MyTree = (TTree*)File->Get("hionia/myTree");
  int nevents= (int)MyTree->GetEntries();
  cout<<"nevents = "<<nevents<<"\n";
   
  TBranch *b_Gen_QQ_size = MyTree->GetBranch("Gen_QQ_size");
  b_Gen_QQ_size->SetAddress(&Gen_QQ_size);

  TBranch *b_Gen_QQ_mupl_idx = MyTree->GetBranch("Gen_QQ_mupl_idx");
  b_Gen_QQ_mupl_idx->SetAddress(&Gen_QQ_mupl_idx);

  TBranch *b_Gen_QQ_mumi_idx = MyTree->GetBranch("Gen_QQ_mumi_idx");
  b_Gen_QQ_mumi_idx->SetAddress(&Gen_QQ_mumi_idx);

  TBranch *b_Gen_QQ_whichRec = MyTree->GetBranch("Gen_QQ_whichRec");
  b_Gen_QQ_whichRec->SetAddress(&Gen_QQ_whichRec);

  TBranch *b_Reco_QQ_size = MyTree->GetBranch("Reco_QQ_size");
  b_Reco_QQ_size->SetAddress(&Reco_QQ_size);

  TBranch *b_Reco_QQ_mupl_idx = MyTree->GetBranch("Reco_QQ_mupl_idx");
  b_Reco_QQ_mupl_idx->SetAddress(&Reco_QQ_mupl_idx);

  TBranch *b_Reco_QQ_mumi_idx = MyTree->GetBranch("Reco_QQ_mumi_idx");
  b_Reco_QQ_mumi_idx->SetAddress(&Reco_QQ_mumi_idx);

  TBranch *b_Reco_mu_SelectionType = MyTree->GetBranch("Reco_mu_SelectionType");
  b_Reco_mu_SelectionType->SetAddress(&Reco_mu_SelectionType);

  TBranch *b_Reco_mu_nTrkWMea = MyTree->GetBranch("Reco_mu_nTrkWMea");
  b_Reco_mu_nTrkWMea->SetAddress(&Reco_mu_nTrkWMea);

  TBranch *b_Reco_mu_nPixWMea = MyTree->GetBranch("Reco_mu_nPixWMea");
  b_Reco_mu_nPixWMea->SetAddress(&Reco_mu_nPixWMea);

  TBranch *b_Reco_mu_dxy = MyTree->GetBranch("Reco_mu_dxy");
  b_Reco_mu_dxy->SetAddress(&Reco_mu_dxy);

  TBranch *b_Reco_mu_dz = MyTree->GetBranch("Reco_mu_dz");
  b_Reco_mu_dz->SetAddress(&Reco_mu_dz);

  TBranch *b_Reco_QQ_sign = MyTree->GetBranch("Reco_QQ_sign");
  b_Reco_QQ_sign->SetAddress(&Reco_QQ_sign);

  TBranch *b_Reco_QQ_trig = MyTree->GetBranch("Reco_QQ_trig");
  b_Reco_QQ_trig->SetAddress(&Reco_QQ_trig);

  TBranch *b_Reco_mu_trig = MyTree->GetBranch("Reco_mu_trig");
  b_Reco_mu_trig->SetAddress(&Reco_mu_trig);

  TBranch *b_HLTriggers = MyTree->GetBranch("HLTriggers");
  b_HLTriggers->SetAddress(&HLTriggers);

  TBranch *b_Reco_QQ_VtxProb = MyTree->GetBranch("Reco_QQ_VtxProb");
  b_Reco_QQ_VtxProb->SetAddress(&Reco_QQ_VtxProb);

  //4-moms
  TBranch *b_Gen_QQ_4mom;
  TClonesArray *Gen_QQ_4mom = new TClonesArray();
  MyTree->SetBranchAddress("Gen_QQ_4mom", &Gen_QQ_4mom, &b_Gen_QQ_4mom);
   
  TBranch *b_Gen_mu_4mom;
  TClonesArray *Gen_mu_4mom = new TClonesArray();
  MyTree->SetBranchAddress("Gen_mu_4mom", &Gen_mu_4mom, &b_Gen_mu_4mom);

  TBranch *b_Reco_QQ_4mom;
  TClonesArray *Reco_QQ_4mom = new TClonesArray();
  MyTree->SetBranchAddress("Reco_QQ_4mom", &Reco_QQ_4mom, &b_Reco_QQ_4mom);

  TBranch *b_Reco_mu_4mom;
  TClonesArray *Reco_mu_4mom = new TClonesArray();
  MyTree->SetBranchAddress("Reco_mu_4mom", &Reco_mu_4mom, &b_Reco_mu_4mom);

  Long64_t nentries = nevents;
  
  for (Long64_t jentry=0; jentry<nentries;jentry++)//Loop on ALL events
    //for (Long64_t jentry=0; jentry<200000;jentry++)//Loop on first 10000 events
    {
        if( (jentry % 100000)==0 ) cout<<"Reading event "<<jentry<<endl;

      //MyTree->GetEntry(jEntry);
      if(jentry%nentries==0){ cout<<"Part of tree scanned : "<< 100*(double)jentry / (double)nentries<<" % \n";}

      Gen_QQ_4mom->Clear();
      Gen_mu_4mom->Clear();
      Reco_QQ_4mom->Clear();
      Reco_mu_4mom->Clear();

      b_Gen_QQ_size->GetEntry(jentry);
      if(Gen_QQ_size > 0)
	{
	  b_Gen_QQ_mupl_idx->GetEntry(jentry);
	  b_Gen_QQ_mumi_idx->GetEntry(jentry);
	  b_Gen_QQ_4mom->GetEntry(jentry);
	  b_Gen_mu_4mom->GetEntry(jentry);
	  b_Gen_QQ_whichRec->GetEntry(jentry);
	}

      b_Reco_QQ_size->GetEntry(jentry);
      if(Reco_QQ_size > 0)
	{
	  b_Reco_QQ_mupl_idx->GetEntry(jentry);
	  b_Reco_QQ_mumi_idx->GetEntry(jentry);
	  b_Reco_QQ_4mom->GetEntry(jentry);
	  b_Reco_mu_4mom->GetEntry(jentry);
	  b_Reco_mu_SelectionType->GetEntry(jentry);
	  //b_Reco_mu_whichGen->GetEntry(jentry);
	  b_Reco_mu_nTrkWMea->GetEntry(jentry);
	  b_Reco_mu_nPixWMea->GetEntry(jentry);
	  b_Reco_mu_dxy->GetEntry(jentry);
	  b_Reco_mu_dz->GetEntry(jentry);
	  b_Reco_QQ_sign->GetEntry(jentry);
	  b_Reco_QQ_trig->GetEntry(jentry);
	  b_Reco_mu_trig->GetEntry(jentry);
	  b_HLTriggers->GetEntry(jentry);
	  b_Reco_QQ_VtxProb->GetEntry(jentry);
	}

      for(int iQQ=0; iQQ < Gen_QQ_size; iQQ++)//Loop on generated upsilon
	{
	 TLorentzVector *GenQQ = (TLorentzVector*) Gen_QQ_4mom->At(iQQ);
	 float GenUpsilonPt = GenQQ->Pt();
	 float GenUpsilonRapidity = GenQQ->Rapidity();
	 float GenUpsilonEta = GenQQ->Eta();

	 if( GenUpsilonPt > 50.0 ) continue;
	 
	 TLorentzVector *GenQQmuPos = (TLorentzVector*) Gen_mu_4mom->At(Gen_QQ_mupl_idx[iQQ]);
	 TLorentzVector *GenQQmuNeg = (TLorentzVector*) Gen_mu_4mom->At(Gen_QQ_mumi_idx[iQQ]);
	 float MuNegGenPt = GenQQmuNeg->Pt();
	 float MuPosGenPt = GenQQmuPos->Pt();
	 
	 float MuNegGenEta = GenQQmuNeg->Eta();
	 float MuPosGenEta = GenQQmuPos->Eta();

	 //Defining EFficiency denominator conditions as booleans
	 bool GenUpsilonRapidityMax = abs(GenUpsilonRapidity) < 2.4;

	 bool MuPosGenPtMin = MuPosGenPt > 3.5;
	 bool MuPosGenEtaMax = abs(MuPosGenEta) < 2.4;

	 bool MuNegGenPtMin = MuNegGenPt > 3.5;
	 bool MuNegGenEtaMax = abs(MuNegGenEta) < 2.4;

	 bool MuonPtMin = ( MuPosGenPtMin && MuNegGenPtMin );
	 bool MuonEtaMax = ( MuPosGenEtaMax && MuNegGenEtaMax );

	 //Get weight value using weight function
	 Double_t WeightVal = weightF(GenUpsilonPt);

	 //Check for conditions and fill denominator histograms
	 if( (GenUpsilonRapidityMax == false) || (MuonPtMin == false) || (MuonEtaMax == false) ) continue;

	 hGenUpsilonEffDen->Fill(GenUpsilonPt,GenUpsilonRapidity);
         hGenUpsilonEffDenW->Fill(GenUpsilonPt,GenUpsilonRapidity,(1/WeightVal));

	 //Check reco event
	 int isReco = Gen_QQ_whichRec[iQQ];

	 if( isReco < 0 ) continue;

	 int iMuPos = Reco_QQ_mupl_idx[isReco];
	 int iMuNeg = Reco_QQ_mumi_idx[isReco];

	 int isMuGenPos = Reco_mu_whichGen[iMuPos];
	 int isMuGenNeg = Reco_mu_whichGen[iMuNeg];

	 if( (isMuGenPos < 0) || (isMuGenNeg < 0) ) continue;

	 TLorentzVector *RecoQQ_MuPos = (TLorentzVector*) Reco_mu_4mom->At(iMuPos);
	   TLorentzVector *RecoQQ_MuNeg = (TLorentzVector*) Reco_mu_4mom->At(iMuNeg);

	   float RecoMuPosPt = RecoQQ_MuPos->Pt();
	   float RecoMuPosEta = RecoQQ_MuPos->Eta();
	   float RecoMuNegPt = RecoQQ_MuNeg->Pt();
	   float RecoMuNegEta = RecoQQ_MuNeg->Eta();

	   //Defining muon numerator conditions as booleans
	   
	   //Defining pT conditions as booleans
	   bool RecoMuPosPtMin = RecoMuPosPt > 3.5;
	   bool RecoMuNegPtMin = RecoMuNegPt > 3.5;
	   if( (RecoMuPosPtMin == false) || (RecoMuNegPtMin == false) ) continue;

	   //Defining Eta conditions as booleans
	   bool RecoMuPosEtaMin = abs(RecoMuPosEta) < 2.4;
	   bool RecoMuNegEtaMin = abs(RecoMuNegEta) < 2.4;
	   if( (RecoMuPosEtaMin == false) || (RecoMuNegEtaMin == false) ) continue;
	   
	   //Check if both muons are Global Muons
	   bool IsGlobalMuPos = (Reco_mu_SelectionType[iMuPos]&((ULong64_t)pow(2, 1))) > 0;
	   bool IsGlobalMuNeg = (Reco_mu_SelectionType[iMuNeg]&((ULong64_t)pow(2, 1))) > 0;
	   if ( (IsGlobalMuPos == false) || (IsGlobalMuNeg == false) ) continue;
	   
	   //Check if both muons are Tracker Muons
	   bool IsTrackerMuPos = (Reco_mu_SelectionType[iMuPos]&((ULong64_t)pow(2, 3))) > 0;
	   bool IsTrackerMuNeg = (Reco_mu_SelectionType[iMuNeg]&((ULong64_t)pow(2, 3))) > 0;
	   if ( (IsTrackerMuPos == false) || (IsTrackerMuNeg == false) ) continue;

	   //Check if both muons have at least 6 silicon tracker hits
	   bool nTrkWMeaMinPos = ( Reco_mu_nTrkWMea[iMuPos]  > 5 );
	   bool nTrkWMeaMinNeg = ( Reco_mu_nTrkWMea[iMuNeg] > 5 );
	   if( (nTrkWMeaMinPos == false) || (nTrkWMeaMinNeg == false) ) continue;

	   //Check if both muons have at least 1 silicon pixel hit
	   bool nPixWMeaMinPos = ( Reco_mu_nPixWMea[iMuPos] > 0 );
	   bool nPixWMeaMinNeg = ( Reco_mu_nPixWMea[iMuNeg] > 0 );
	   if( (nPixWMeaMinPos == false) || (nPixWMeaMinNeg == false) ) continue;

	   //Check if both muons' distance from the muon track to the
	   //closest primary vertex is less than 3mm in the transverse direction
	   bool dxyMuPos = ( abs(Reco_mu_dxy[iMuPos]) < 0.3 );
	   bool dxyMuNeg = ( abs(Reco_mu_dxy[iMuNeg]) < 0.3 );
	   if( (dxyMuPos == false) || (dxyMuNeg == false) ) continue;

	   //Check if both muons' distance from the muon track to the
	   //closest primary vertex is less than 20cm in the longitudinal dir.
	   bool dzMuPos = ( abs(Reco_mu_dz[iMuPos]) < 20.0 );
	   bool dzMuNeg = ( abs(Reco_mu_dz[iMuNeg]) < 20.0 );
	   if( ( dzMuPos == false) || (dzMuNeg == false) ) continue;

	   //Check Reconstructed Upsilon
	   TLorentzVector *RecoQQ = (TLorentzVector*) Reco_QQ_4mom->At(isReco);

	   float RecoQQPt = RecoQQ->Pt();
	   float RecoQQRap = RecoQQ->Rapidity();
	   float RecoQQMass = RecoQQ->M();
	   float RecoQQVtxProb = Reco_QQ_VtxProb[isReco];
	   int RecoQQCharge = Reco_QQ_sign[isReco];

	   //Defining Reconstructed Upsilon conditions as a boolean
	   bool RecoQQAcc = ( (RecoQQPt < 100.0) && (RecoQQVtxProb > 0.01) && (RecoQQMass> 8.5) && (RecoQQMass < 11.0) && (RecoQQCharge == 0) );
	   if( RecoQQAcc == false ) continue;

	   int TriggerBit = 3;
	   //Define trigger conditions as booleans
	   bool JpsiHLTtrigers = (HLTriggers&((ULong64_t)pow(2, TriggerBit))) > 0;
	   bool JpsiTrigger = (Reco_QQ_trig[isReco]&((ULong64_t)pow(2, TriggerBit))) > 0;
	   if(JpsiHLTtrigers == false || JpsiTrigger == false) continue;
	   //Check for conditions and fill numerator histograms
           hGenUpsilonEffNum->Fill(GenUpsilonPt,GenUpsilonRapidity);
           hGenUpsilonEffNumW->Fill(GenUpsilonPt,GenUpsilonRapidity,(1/WeightVal));
	 
	 
	}
    }

  //Calculate Efficiency by dividing histograms
  TH2F *hGenUpsilonEff = new TH2F(*hGenUpsilonEffNum); hGenUpsilonEff->Divide(hGenUpsilonEffDen);
  TH2F *hGenUpsilonEffW  = new TH2F(*hGenUpsilonEffNumW); hGenUpsilonEffW->Divide(hGenUpsilonEffDenW);

  //Calculate weighted to unweighted ratio
  TH2F *hGenUpsilonEffWtoUWratio  = new TH2F(*hGenUpsilonEffW); hGenUpsilonEffWtoUWratio->Divide(hGenUpsilonEff);

  //Set histograms parameters and write them out on root file
  TFile *fOut = new TFile(outfilename,"RECREATE");
   hGenUpsilonEff->SetStats(0);
   hGenUpsilonEff->SetContour(1000);
   hGenUpsilonEff->SetTitle("Efficiency Upsilon pT vs y distribution");
   hGenUpsilonEff->SetName("hGenUpsilonEff");
   hGenUpsilonEff->SetTitleSize(1);
   hGenUpsilonEff->GetXaxis()->SetTitleSize(0.06);
   hGenUpsilonEff->GetXaxis()->SetTitleOffset(0.65);
   hGenUpsilonEff->GetXaxis()->SetLabelSize(0.04);
   hGenUpsilonEff->GetXaxis()->SetTitle("Upsilon pT (GeV)");
   hGenUpsilonEff->GetYaxis()->SetTitle("Upsilon y");
   hGenUpsilonEff->GetYaxis()->SetTitleSize(0.06);
   hGenUpsilonEff->GetYaxis()->SetTitleOffset(0.65);
   hGenUpsilonEff->GetYaxis()->SetLabelSize(0.04);
   hGenUpsilonEff->SetOption("colz");
   
   hGenUpsilonEffNum->Write();
   hGenUpsilonEffDen->Write();
   hGenUpsilonEff->Write();

   hGenUpsilonEffW->SetStats(0);
   hGenUpsilonEffW->SetContour(1000);
   hGenUpsilonEffW->SetTitle("Weighted Efficiency Upsilon pT vs y distribution");
   hGenUpsilonEffW->SetTitleSize(1);
   hGenUpsilonEffW->SetName("hGenUpsilonPtvsRapNominal");
   hGenUpsilonEffW->GetXaxis()->SetTitleSize(0.06);
   hGenUpsilonEffW->GetXaxis()->SetTitleOffset(0.65);
   hGenUpsilonEffW->GetXaxis()->SetLabelSize(0.04);
   hGenUpsilonEffW->GetXaxis()->SetTitle("Upsilon pT (GeV)");
   hGenUpsilonEffW->GetYaxis()->SetTitle("Upsilon y");
   hGenUpsilonEffW->GetYaxis()->SetTitleSize(0.06);
   hGenUpsilonEffW->GetYaxis()->SetTitleOffset(0.65);
   hGenUpsilonEffW->GetYaxis()->SetLabelSize(0.04);
   hGenUpsilonEffW->SetOption("colz");
   
   hGenUpsilonEffNumW->Write();
   hGenUpsilonEffDenW->Write();
   hGenUpsilonEffW->Write();

   hGenUpsilonEffWtoUWratio->SetStats(0);
   hGenUpsilonEffWtoUWratio->SetContour(1000);
   hGenUpsilonEffWtoUWratio->SetTitle("2D Efficiency Weighted to Unweighted Ratio");
   hGenUpsilonEffWtoUWratio->SetName("hGenUpsilonEffWtoUWratio");
   hGenUpsilonEffWtoUWratio->GetXaxis()->SetTitleSize(0.06);
   hGenUpsilonEffWtoUWratio->GetXaxis()->SetTitleOffset(0.65);
   hGenUpsilonEffWtoUWratio->GetXaxis()->SetLabelSize(0.04);
   hGenUpsilonEffWtoUWratio->GetXaxis()->SetTitle("Upsilon pT (GeV)");
   hGenUpsilonEffWtoUWratio->GetYaxis()->SetTitle("Upsilon y");
   hGenUpsilonEffWtoUWratio->GetYaxis()->SetTitleSize(0.06);
   hGenUpsilonEffWtoUWratio->GetYaxis()->SetTitleOffset(0.65);
   hGenUpsilonEffWtoUWratio->GetYaxis()->SetLabelSize(0.04);
   hGenUpsilonEffWtoUWratio->SetOption("colz");
   
   hGenUpsilonEffWtoUWratio->Write();
   
  fOut->Close();
}

Bool_t EfficiencyFineBinned::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}