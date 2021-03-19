
#include "AccEffResults.h"
#include <iostream>
#include "TFile.h"
#include "TH1.h"
#include "TEfficiency.h"
#include "../Utils/utils.h"
#include "AccAnalyzer.h"
#include "EffAnalyzer.h"
#include "TGraphAsymmErrors.h"

TH1F* toTH1F(const TEfficiency* asym);
std::unique_ptr<TEfficiency> getAccXEff(TFile* accFile, TFile* effFile);
TH1F* calcDN_DpT(TH1F* nSigCorrected);
TH1F* calcCorrectedYields(TH1F* nSig,TEfficiency* AccXEff);

void AccEffResults(const char* accFilename, const char* effFilename, const char* fitFilepath, const char* outputname)
{
    std::cout << "\nACCEPTANCY EFFICIENCY RESULT TEST\n";

    //input file
    std::cout << "Reading acceptancy input file: " << accFilename <<'\n';
    TFile* accFile = OpenFile(accFilename,"READ");
    //input file
    std::cout << "Reading effiency input file: " << effFilename <<'\n';
    TFile* effFile = OpenFile(effFilename,"READ");
    //input file
    std::cout << "Reading multifit file: " << fitFilepath <<'\n';
    TFile* fitFile = OpenFile(fitFilepath,"READ");
    //reading file
    std::cout << "Writing to file: " << outputname <<'\n';
    TFile* outFile = OpenFile(outputname,"RECREATE");

    std::unique_ptr<TEfficiency> AccXEff = getAccXEff(accFile,effFile);
    AccXEff->Write();
    std::string outbasename= ReplaceExtension(outputname,"");
    writeToCanvasEff(AccXEff.get(),"p_{T}","Acc x Eff",outbasename+"_AccXEff.pdf");
    
    TH1F* nSig = (TH1F*) fitFile->Get("nSigY1S");

    TH1F* nSigCorrected = calcCorrectedYields(nSig,AccXEff.get());
    writeToCanvas(nSigCorrected,"p^{#mu#mu}_{T} GeV/c","N_{Y1Scorr}",outbasename+"_CorrectYields.pdf");
    nSigCorrected->Write();

    TH1F* dN_dPt = calcDN_DpT(nSigCorrected);
    writeToCanvas(dN_dPt,"p^{#mu#mu}_{T} GeV/c","B(#Upsilon #rightarrow #mu #mu) #frac{#sigma}{dp_{T} dy} nb^{-1}",outbasename+"_dNdPt.pdf",true);
    dN_dPt->Write();

    outFile->Close();
    accFile->Close();
    effFile->Close();
    fitFile->Close();
}

 std::unique_ptr<TEfficiency> getAccXEff(TFile* accFile, TFile* effFile)
 {
    TH1F* acc_num= (TH1F*) accFile->Get(accNumName);
    TH1F* eff_num= (TH1F*) effFile->Get(effNumName);
    TH1F* acc_den= (TH1F*) accFile->Get(accDenName);
    TH1F* eff_den= (TH1F*) effFile->Get(effDenName);

    TH1F* dens= new TH1F((*acc_den)*(*eff_den));
    TH1F* nums= new TH1F((*acc_num)*(*eff_num));

    std::unique_ptr<TEfficiency> AccXEff = createTEff(nums,dens,"pt QQ AccXEff");
    return AccXEff;
 }

TH1F* toTH1F(const TEfficiency* asym)
{
    int nBins= asym->GetPassedHistogram()->GetNbinsX()+2;
    TH1F* result = new TH1F(*dynamic_cast<TH1F*>(asym->GetPassedHistogram()->Clone()));
    for(int i=0;i< nBins;i++)
    {
        result->SetBinContent(i,asym->GetEfficiency(i));
        float upErr= asym->GetEfficiencyErrorUp(i);
        float downErr = asym->GetEfficiencyErrorLow(i);
        result->SetBinError(i, (upErr > downErr) ? upErr : downErr );
    }
    return result;
}

TH1F* calcCorrectedYields(TH1F* nSig,TEfficiency* AccXEff)
{
    TH1F* AccXEffth1f=toTH1F(AccXEff);
    TH1F* nSigCorrected = new TH1F((*nSig)/(*AccXEffth1f));
    nSigCorrected->SetName("corrected_nSigY1S");
    nSigCorrected->SetTitle("nSigY1S corrected");
    return nSigCorrected;
}

TH1F* calcDN_DpT(TH1F* nSigCorrected)
{
    const float dy= 4.8f;
    const float L=28000.0f;//in nb^-1
    TH1F* dN_dPt = new TH1F(*nSigCorrected);
    int nbins = dN_dPt->GetNbinsX() +2;
    for(int i=0;i<nbins;i++)
    {
        float dPt= dN_dPt->GetBinWidth(i);
        float dN= dN_dPt->GetBinContent(i);
        float factor=1.0f/(dPt*dy);
        float value=dN*factor;
        dN_dPt->SetBinContent(i, value);
        dN_dPt->SetBinError(i, nSigCorrected->GetBinError(i)*factor );
    }
    dN_dPt->Scale(1.0f/L);
    dN_dPt->SetName("dN_dpTdy");
    dN_dPt->SetTitle("#frac{dN_{Y1Scorr}}{L #Delta p_{T} #Delta y}");
    dN_dPt->GetYaxis()->SetRangeUser(0.0001f,10.0f);
    return dN_dPt;
}