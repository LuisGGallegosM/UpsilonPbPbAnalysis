
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
    writeToCanvasEff(AccXEff.get(),"p_{T}","Acc x Eff",outbasename+"_AccXEff.pdf");
    
    TH1F* nSigRealData = (TH1F*) fitFileRealData->Get("nSigY1S");

    TH1F* nSigCorrected = calcCorrectedYields(nSigRealData,AccXEff.get());
    writeToCanvas(nSigCorrected,"p^{#mu#mu}_{T} GeV/c","N_{Y1Scorr}",outbasename+"_CorrectYields.pdf");
    nSigCorrected->Write();

    TH1F* DATA_dN_dPt = Normalize(nSigCorrected);
    DATA_dN_dPt->SetName("DATA_dN_dpT_norm");
    DATA_dN_dPt->SetTitle("DATA dN/dp_{T} normalized");
    DATA_dN_dPt->SetFillColor(3);
    DATA_dN_dPt->SetLineColor(3);
    DATA_dN_dPt->GetYaxis()->SetRangeUser(0.0,1.0);

    TH1F* nSigMC = (TH1F*) fitFileMC->Get("nSigY1S");
    TH1F* MC_dN_dPt =Normalize(nSigMC);
    MC_dN_dPt->SetName("MC_dN_dpT_norm");
    MC_dN_dPt->SetTitle("MC dN/dp_{T} normalized");
    MC_dN_dPt->SetFillColor(1);
    MC_dN_dPt->SetLineColor(1);
    MC_dN_dPt->GetYaxis()->SetRangeUser(0.0,1.0);

    std::vector<TH1*> hists ={DATA_dN_dPt,MC_dN_dPt};
    writeToCanvas(hists,"p^{#mu#mu}_{T} GeV/c","#frac{#sigma}{dp_{T}} nb^{-1}",outbasename+"_dNdPt.pdf");
    DATA_dN_dPt->Write();
    MC_dN_dPt->Write();

    TH1F* ratio= new TH1F((*MC_dN_dPt)/(*DATA_dN_dPt));
    ratio->SetName("ratio_dN_dpT_norm");
    ratio->SetTitle("#frac{MC dN/dp_{T}}{DATA dN/dp_{T}}");
    ratio->GetYaxis()->SetRangeUser(0.0,3.0);
    ratio->SetFillColor(4);
    writeToCanvas(ratio,"p^{#mu#mu}_{T} GeV/c","ratio", outbasename+"_dNdPtRatio.pdf");
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

    std::unique_ptr<TEfficiency> AccXEff = createTEff(nums,dens,"pt QQ AccXEff","#alpha #epsilon;p^{#mu#mu}_{T} GeV/c; #alpha#epsilon;");
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
    nSigCorrected->SetStats(false);
    return nSigCorrected;
}

TH1F* Normalize(TH1F* nSigCorrected)
{
    TH1F* dN_dPt = new TH1F(*nSigCorrected);
    int nbins = dN_dPt->GetNbinsX() +2;
    float sum = 0.0f;
    for(int i=0;i<nbins;i++)
    {
        sum+=dN_dPt->GetBinContent(i);
    }
    dN_dPt->Scale(1.0f/sum);
    return dN_dPt;
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