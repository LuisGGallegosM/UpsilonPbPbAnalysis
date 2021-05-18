
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
std::unique_ptr<TEfficiency> getAccXEff(TFile* accFile, TFile* effFile, const std::string& outbasename);
TH1F* calcDN_DpT(TH1F* nSigCorrected);
TH1F* Normalize(TH1F* nSigCorrected);
TH1F* generateRefCrossSection(const float* error);

void getCrossSections(const TH1F* DATA_dN_dPt, const std::string& outbasename);
void getRatios(const TH1F* DATA_dN_dPt_norm, const TH1F* MC_dN_dPt_norm, const std::string& outbasename );
TH1F* getNSigMC(TFile* accFile, const std::string& outbasename);

const int colorGraph[2] = {4 ,2 } ;

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

    std::string outbasename= ReplaceExtension(outputname,"");

    std::unique_ptr<TEfficiency> AccXEff = getAccXEff(accFile,effFile,outbasename);
    
    TH1F* nSigRealData = (TH1F*) fitFileRealData->Get(nSigY1SName);
    nSigRealData->SetName("DATA_nSigY1S");

    TH1F* nSigCorrected = calcCorrectedYields(nSigRealData,AccXEff.get());
    writeToCanvas(nSigCorrected,"p^{#mu#mu}_{T} GeV/c","nSigY1S",outbasename);
    nSigCorrected->Write();

    TH1F* DATA_dN_dPt = calcDN_DpT(nSigCorrected);

    getCrossSections(DATA_dN_dPt,outbasename);

    TH1F* DATA_dN_dPt_norm = Normalize(DATA_dN_dPt);
    DATA_dN_dPt_norm->SetLineColor(colorGraph[0]);
    DATA_dN_dPt_norm->SetTitle("DATA");

    TH1F* nSigMC_gen = getNSigMC(accFile,outbasename);

    TH1F* MC_dN_dPt= calcDN_DpT(nSigMC_gen);
    TH1F* MC_dN_dPt_norm =Normalize(MC_dN_dPt);
    MC_dN_dPt_norm->SetLineColor(colorGraph[1]);
    MC_dN_dPt_norm->SetTitle("MC gen");

    std::vector<TH1*> hists ={DATA_dN_dPt_norm,MC_dN_dPt_norm};

    writeToCanvas(hists,"#frac{dN}{dp_{T}} normalized",xAxisLabel," ",outbasename+"_dNdPt.pdf");

    DATA_dN_dPt_norm->Write();
    MC_dN_dPt_norm->Write();

    getRatios(DATA_dN_dPt_norm,MC_dN_dPt_norm,outbasename);    

    outFile->Close();
    accFile->Close();
    effFile->Close();
    fitFileRealData->Close();
}

TH1F* getNSigMC(TFile* accFile, const std::string& outbasename)
{
    TH1F* nSigMC_gen = (TH1F*) accFile->Get(accDenName);
    nSigMC_gen->SetName("MC_nSigY1S_gen");

    writeToCanvas(nSigMC_gen,xAxisLabel,"nSigY1S",outbasename);
    nSigMC_gen->Write();
    return nSigMC_gen;
}

void getCrossSections(const TH1F* DATA_dN_dPt, const std::string& outbasename)
{
    TH1F* cross_section = new TH1F(*DATA_dN_dPt);
    cross_section->SetName("cross_section");
    cross_section->Scale((1.0f)/(yRange*integratedLum));
    cross_section->SetTitle("calculated");

    TH1F* ref_cross_section_sys = generateRefCrossSection(ref_cross_section_error_sys.data());
    ref_cross_section_sys->SetLineColor(colorGraph[1]);
    ref_cross_section_sys->SetTitle(AnalysisNoteName);

    TH1F* ref_cross_section_stat = generateRefCrossSection(ref_cross_section_error_stat.data());
    ref_cross_section_stat->SetLineColor(colorGraph[1]);
    ref_cross_section_stat->SetTitle(AnalysisNoteName);
    
    std::vector<TH1*> cross_sec_sys = { cross_section,ref_cross_section_sys};
    std::vector<TH1*> cross_sec_stat = { cross_section,ref_cross_section_stat};

    writeToCanvas(cross_sec_sys,"B #frac{d#sigma}{dp_{T}dy}",xAxisLabel, "B #frac{d#sigma}{dp_{T}dy} ( nb / GeV/c )", outbasename+"_cross_sections_sys.pdf" );
    writeToCanvas(cross_sec_stat,"B #frac{d#sigma}{dp_{T}dy}",xAxisLabel, "B #frac{d#sigma}{dp_{T}dy} ( nb / GeV/c )", outbasename+"_cross_sections_stat.pdf" );
}

void getRatios(const TH1F* DATA_dN_dPt_norm, const TH1F* MC_dN_dPt_norm, const std::string& outbasename )
{
    TH1F* ratio= new TH1F((*DATA_dN_dPt_norm)/(*MC_dN_dPt_norm));
    ratio->SetName(yieldFitName);
    ratio->SetTitle("#frac{DATA dN_{DATAcorr}/dp_{T} norm}{MC dN_{MC}/dp_{T} norm}");
    ratio->SetTitleSize(0.004);
    ratio->SetTitleSize(0.004);
    ratio->SetFillColor(colorGraph[0]);
    ratio->SetLineColor(colorGraph[0]);
    ratio->SetStats(false);
    writeToCanvas(ratio,xAxisLabel,"ratio", outbasename);
    ratio->Write();
}

 std::unique_ptr<TEfficiency> getAccXEff(TFile* accFile, TFile* effFile, const std::string& outbasename)
 {
    TH1F* acc_num= (TH1F*) accFile->Get(accNumName);
    TH1F* eff_num= (TH1F*) effFile->Get(effNumName);
    TH1F* acc_den= (TH1F*) accFile->Get(accDenName);
    TH1F* eff_den= (TH1F*) effFile->Get(effDenName);

    TH1F* dens= new TH1F((*acc_den)*(*eff_den));
    TH1F* nums= new TH1F((*acc_num)*(*eff_num));

    std::unique_ptr<TEfficiency> AccXEff = createTEff(nums,dens,accXEffName,"#alpha #epsilon;"+ std::string(xAxisLabel)+"; #alpha#epsilon;");
    AccXEff->Write();
    writeToCanvasEff(AccXEff.get(),"p_{T}","Acc x Eff",outbasename);
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