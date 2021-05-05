
#include "AccEffResults.h"
#include <iostream>

#include "TFile.h"
#include "TH1.h"
#include "TEfficiency.h"
#include "TGraphAsymmErrors.h"

#include "../Common/AccEffAux.h"
#include "../../Utils/Helpers/Helpers.h"
#include "../../Drawing/Drawing.h"

TH1F* toTH1F(const TEfficiency* asym);
std::unique_ptr<TEfficiency> getAccXEff(TFile* accFile, TFile* effFile);
TH1F* calcDN_DpT(TH1F* nSigCorrected);
TH1F* Normalize(TH1F* nSigCorrected);
TH1F* calcCorrectedYields(TH1F* nSig,TEfficiency* AccXEff);
TH1F* generateRefCrossSection(const float* error);

void AccEffResults(const char* accFilename, const char* effFilename, const char* fitFileRealDataPath, const char* fitFileMCPath, const char* outputname)
{
    std::cout << "\nACCEPTANCY EFFICIENCY RESULT TEST\n";

    //input file
    std::cout << "Reading acceptancy input file: " << accFilename <<'\n';
    TFile* accFile = OpenFile(accFilename,"READ");
    //input file
    std::cout << "Reading effiency input file: " << effFilename <<'\n';
    TFile* effFile = OpenFile(effFilename,"READ");
    //input file
    std::cout << "Reading Real Data multifit file: " << fitFileRealDataPath <<'\n';
    TFile* fitFileRealData = OpenFile(fitFileRealDataPath,"READ");
    //input file
    std::cout << "Reading MC multifit file: " << fitFileMCPath <<'\n';
    TFile* fitFileMC = OpenFile(fitFileMCPath,"READ");
    //output file
    std::cout << "Writing to file: " << outputname <<'\n';
    TFile* outFile = OpenFile(outputname,"RECREATE");

    std::unique_ptr<TEfficiency> AccXEff = getAccXEff(accFile,effFile);
    AccXEff->Write();
    std::string outbasename= ReplaceExtension(outputname,"");
    writeToCanvasEff(AccXEff.get(),"p_{T}","Acc x Eff",outbasename);
    
    TH1F* nSigRealData = (TH1F*) fitFileRealData->Get(nSigY1SName);
    nSigRealData->SetName("DATA_nSigY1S");

    TH1F* nSigCorrected = calcCorrectedYields(nSigRealData,AccXEff.get());
    writeToCanvas(nSigCorrected,"p^{#mu#mu}_{T} GeV/c","nSigY1S",outbasename);
    nSigCorrected->Write();

    TH1F* DATA_dN_dPt = calcDN_DpT(nSigCorrected);

    TH1F* cross_section = new TH1F(*DATA_dN_dPt);
    cross_section->SetName("cross_section");
    cross_section->Scale((1.0f)/(yRange*integratedLum));
    cross_section->SetTitle("calculated");

    TH1F* ref_cross_section_sys = generateRefCrossSection(ref_cross_section_error_sys.data());
    ref_cross_section_sys->SetLineColor(1);
    ref_cross_section_sys->SetTitle("CMS AN -2016/354 sys error");

    TH1F* ref_cross_section_stat = generateRefCrossSection(ref_cross_section_error_stat.data());
    ref_cross_section_stat->SetLineColor(1);
    ref_cross_section_stat->SetTitle("CMS AN -2016/354 stat error");
    
    std::vector<TH1*> cross_sec_sys = { cross_section,ref_cross_section_sys};
    std::vector<TH1*> cross_sec_stat = { cross_section,ref_cross_section_stat};

    writeToCanvas(cross_sec_sys,"B #frac{d#sigma}{dp_{T}dy}","p^{#mu#mu}_{T} GeV/c", "B #frac{d#sigma}{dp_{T}dy} ( nb / GeV/c )", outbasename+"_cross_sections_sys.pdf" );
    writeToCanvas(cross_sec_stat,"B #frac{d#sigma}{dp_{T}dy}","p^{#mu#mu}_{T} GeV/c", "B #frac{d#sigma}{dp_{T}dy} ( nb / GeV/c )", outbasename+"_cross_sections_stat.pdf" );

    TH1F* DATA_dN_dPt_norm = Normalize(DATA_dN_dPt);
    DATA_dN_dPt_norm->SetLineColor(1);

    TH1F* nSigMC_gen = (TH1F*) accFile->Get(accDenName);
    nSigMC_gen->SetName("MC_nSigY1S_gen");

    writeToCanvas(nSigMC_gen,"p^{#mu#mu}_{T} GeV/c","nSigY1S",outbasename);
    nSigMC_gen->Write();

    TH1F* MC_dN_dPt= calcDN_DpT(nSigMC_gen);
    TH1F* MC_dN_dPt_norm =Normalize(MC_dN_dPt);
    MC_dN_dPt_norm->SetLineColor(3);

    std::vector<TH1*> hists ={DATA_dN_dPt_norm,MC_dN_dPt_norm};
    DATA_dN_dPt_norm->SetTitle("DATA");
    MC_dN_dPt_norm->SetTitle("MC gen");
    writeToCanvas(hists,"#frac{dN}{dp_{T}} normalized","p^{#mu#mu}_{T} GeV/c"," ",outbasename+"_dNdPt.pdf");
    DATA_dN_dPt_norm->Write();
    MC_dN_dPt_norm->Write();

    TH1F* ratio= new TH1F((*DATA_dN_dPt_norm)/(*MC_dN_dPt_norm));
    ratio->SetName(yieldFitName);
    ratio->SetTitle("#frac{DATA dN_{DATAcorr}/dp_{T} norm}{MC dN_{MC}/dp_{T} norm}");
    ratio->SetTitleSize(0.004);
    ratio->SetTitleSize(0.004);
    ratio->GetYaxis()->SetRangeUser(0.0,3.5);
    ratio->SetFillColor(1);
    ratio->SetLineColor(1);
    ratio->SetStats(false);
    writeToCanvas(ratio,"p^{#mu#mu}_{T} GeV/c","ratio", outbasename);
    ratio->Write();

    outFile->Close();
    accFile->Close();
    effFile->Close();
    fitFileRealData->Close();
}

 std::unique_ptr<TEfficiency> getAccXEff(TFile* accFile, TFile* effFile)
 {
    TH1F* acc_num= (TH1F*) accFile->Get(accNumName);
    TH1F* eff_num= (TH1F*) effFile->Get(effNumName);
    TH1F* acc_den= (TH1F*) accFile->Get(accDenName);
    TH1F* eff_den= (TH1F*) effFile->Get(effDenName);

    TH1F* dens= new TH1F((*acc_den)*(*eff_den));
    TH1F* nums= new TH1F((*acc_num)*(*eff_num));

    std::unique_ptr<TEfficiency> AccXEff = createTEff(nums,dens,accXEffName,"#alpha #epsilon;p^{#mu#mu}_{T} GeV/c; #alpha#epsilon;");
    return AccXEff;
 }

 TH1F* generateRefCrossSection(const float* error)
 {
     int nbins=pt_bins.size()-1;
     TH1F* ref_cross= new TH1F("ref_cross_section","ref_cross_section",nbins,pt_bins.data());
     for(int i=0;i< nbins;i++)
     {
         ref_cross->SetBinContent(i+1,ref_cross_section_value[i]);
         ref_cross->SetBinError(i+1,error[i]);
     }
     return ref_cross;
 }