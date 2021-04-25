
#include "AccEffResults.h"
#include <iostream>

#include "TFile.h"
#include "TH1.h"
#include "TEfficiency.h"
#include "TGraphAsymmErrors.h"

#include "../Common/AccEffAux.h"
#include "../../Utils/Helpers/Helpers.h"

TH1F* toTH1F(const TEfficiency* asym);
std::unique_ptr<TEfficiency> getAccXEff(TFile* accFile, TFile* effFile);
TH1F* calcDN_DpT(TH1F* nSigCorrected);
TH1F* Normalize(TH1F* nSigCorrected);
TH1F* calcCorrectedYields(TH1F* nSig,TEfficiency* AccXEff);

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
    
    TH1F* nSigRealData = (TH1F*) fitFileRealData->Get("nSigY1S");
    nSigRealData->SetName("DATA_nSigY1S");

    TH1F* nSigCorrected = calcCorrectedYields(nSigRealData,AccXEff.get());
    writeToCanvas(nSigCorrected,"p^{#mu#mu}_{T} GeV/c","N_{Y1Scorr}",outbasename);
    nSigCorrected->Write();

    TH1F* DATA_dN_dPt = calcDN_DpT(nSigCorrected);

    TH1F* DATA_dN_dPt_norm = Normalize(DATA_dN_dPt);
    DATA_dN_dPt_norm->SetLineColor(1);

    TH1F* nSigMC = (TH1F*) fitFileMC->Get("nSigY1S");
    nSigMC->SetName("MC_nSigY1S");

    TH1F* nSigCorrected_MC=calcCorrectedYields(nSigMC,AccXEff.get());
    writeToCanvas(nSigCorrected_MC,"p^{#mu#mu}_{T} GeV/c","N_{Y1Scorr}",outbasename);
    nSigCorrected_MC->Write();

    TH1F* MC_dN_dPt= calcDN_DpT(nSigMC);
    TH1F* MC_dN_dPt_norm =Normalize(MC_dN_dPt);
    MC_dN_dPt_norm->SetLineColor(3);

    std::vector<TH1*> hists ={DATA_dN_dPt_norm,MC_dN_dPt_norm};
    writeToCanvas(hists,"DATA corrected and MC N_{Y1S} Normalized","p^{#mu#mu}_{T} GeV/c","N_{Y1S}",outbasename+"_dNdPt.pdf");
    DATA_dN_dPt_norm->Write();
    MC_dN_dPt_norm->Write();

    TH1F* ratio= new TH1F((*MC_dN_dPt_norm)/(*DATA_dN_dPt_norm));
    ratio->SetName(yieldFitName);
    ratio->SetTitle("#frac{MC N_{Y1SMC} norm}{DATA N_{Y1Scorr} norm}");
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