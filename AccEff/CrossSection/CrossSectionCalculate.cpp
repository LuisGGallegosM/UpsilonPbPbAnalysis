
#include"CrossSectionCalculate.h"

#include <iostream>

#include "TFile.h"
#include "TH1.h"
#include "TEfficiency.h"

#include "../Common/AccEffAux.h"
#include "../../Utils/Helpers/Helpers.h"
#include "../../Drawing/Drawing.h"

void getCrossSections(const TH1D* DATA_dN_dPt, const std::string& outbasename);
TH1D* generateRefCrossSection(const float* error);
TH1D* getRatios(const TH1D* DATA_dN_dPt_norm, const TH1D* MC_dN_dPt_norm, const std::string& outbasename );
TH1D* getNSigMC(TFile* accFile, const std::string& outbasename);
TH1D* toTH1D(const TEfficiency* asym);
TH1D* calcDN_DpT(TH1D* nSigCorrected);
TH1D* Normalize(TH1D* nSigCorrected);

const int colorGraph[2] = {4 ,2 } ;
const char xAxisLabel[] ="p^{#mu#mu}_{T} GeV/c";

const bool alreadyCorrected = false;

void CrossSectionCalculate(const char* acceffFilename, const char* fitFileRealDataPath, const char* fitFileMCPath, const char* outputname)
{
    std::cout << "\nACCEPTANCY EFFICIENCY RESULT TEST\n";

    TFile* acceffFile = nullptr;

    //input file
    if (alreadyCorrected)
    {
        std::cout << "File Already accXEff corrected\n";
    } else
    {
        std::cout << "Reading acceptancy x efficiency input file: " << acceffFilename <<'\n';
        acceffFile = OpenFile(acceffFilename,"READ");
    }

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

    TH1D* nSigRealData = (TH1D*) fitFileRealData->Get(nSigY1SName);
    nSigRealData->SetName("DATA_nSigY1S");

    TH1D* DATA_dN_dPt =nullptr;
    if(alreadyCorrected)
    {
        TH1D* nSigCorrected = nSigRealData;
        writeToCanvas(nSigCorrected,"p^{#mu#mu}_{T} GeV/c","nSigY1S",outbasename);
        nSigCorrected->Write();

        DATA_dN_dPt = calcDN_DpT(nSigCorrected);
    } else
    {
        TEfficiency* AccXEff= (TEfficiency*) acceffFile->Get(accXEffName);

        TH1D* nSigCorrected = calcCorrectedYields(nSigRealData,AccXEff);
        writeToCanvas(nSigCorrected,"p^{#mu#mu}_{T} GeV/c","nSigY1S",outbasename);
        nSigCorrected->Write();

        DATA_dN_dPt = calcDN_DpT(nSigCorrected);
    }

    getCrossSections(DATA_dN_dPt,outbasename);

    TH1D* DATA_dN_dPt_norm = Normalize(DATA_dN_dPt);
    DATA_dN_dPt_norm->SetLineColor(colorGraph[0]);
    DATA_dN_dPt_norm->SetTitle("DATA");

    TH1D* nSigMC_gen = (TH1D*) fitFileMC->Get(nSigY1SName);

    TH1D* MC_dN_dPt= calcDN_DpT(nSigMC_gen);
    TH1D* MC_dN_dPt_norm =Normalize(MC_dN_dPt);
    MC_dN_dPt_norm->SetLineColor(colorGraph[1]);
    MC_dN_dPt_norm->SetTitle("MC gen");

    std::vector<TH1*> hists ={DATA_dN_dPt_norm,MC_dN_dPt_norm};

    writeToCanvas(hists,"#frac{dN}{dp_{T}} normalized",xAxisLabel," ",outbasename+"_dNdPt.pdf");

    DATA_dN_dPt_norm->Write();
    MC_dN_dPt_norm->Write();

    TH1D* ratio= getRatios(DATA_dN_dPt_norm,MC_dN_dPt_norm,outbasename);
    writeToCanvas(ratio,xAxisLabel,"ratio", outbasename);
    ratio->Write();

    outFile->Close();
    if (acceffFile) acceffFile->Close();
    fitFileRealData->Close();
}

void getCrossSections(const TH1D* DATA_dN_dPt, const std::string& outbasename)
{
    TH1D* cross_section = new TH1D(*DATA_dN_dPt);
    cross_section->SetName("cross_section");
    cross_section->Scale((1.0f)/(yRange*integratedLum));
    cross_section->SetTitle("calculated");

    TH1D* ref_cross_section_sys = generateRefCrossSection(ref_cross_section_error_sys.data());
    ref_cross_section_sys->SetLineColor(colorGraph[1]);
    ref_cross_section_sys->SetTitle(AnalysisNoteName);

    TH1D* ref_cross_section_stat = generateRefCrossSection(ref_cross_section_error_stat.data());
    ref_cross_section_stat->SetLineColor(colorGraph[1]);
    ref_cross_section_stat->SetTitle(AnalysisNoteName);
    
    std::vector<TH1*> cross_sec_sys = { cross_section,ref_cross_section_sys};
    std::vector<TH1*> cross_sec_stat = { cross_section,ref_cross_section_stat};

    writeToCanvas(cross_sec_sys,"B #frac{d#sigma}{dp_{T}dy}",xAxisLabel, "B #frac{d#sigma}{dp_{T}dy} ( nb / GeV/c )", outbasename+"_cross_sections_sys.pdf" );
    writeToCanvas(cross_sec_stat,"B #frac{d#sigma}{dp_{T}dy}",xAxisLabel, "B #frac{d#sigma}{dp_{T}dy} ( nb / GeV/c )", outbasename+"_cross_sections_stat.pdf" );
}

TH1D* getRatios(const TH1D* DATA_dN_dPt_norm, const TH1D* MC_dN_dPt_norm, const std::string& outbasename )
{
    TH1D* ratio= new TH1D((*DATA_dN_dPt_norm)/(*MC_dN_dPt_norm));
    ratio->SetName(yieldFitName);
    ratio->SetTitle("#frac{DATA dN_{DATAcorr}/dp_{T} norm}{MC dN_{MC}/dp_{T} norm}");
    ratio->SetTitleSize(0.004);
    ratio->SetTitleSize(0.004);
    ratio->SetFillColor(colorGraph[0]);
    ratio->SetLineColor(colorGraph[0]);
    ratio->SetStats(false);
    return ratio;
}

 TH1D* generateRefCrossSection(const float* error)
 {
     int nbins=pt_bins.size()-1;
     TH1D* ref_cross= new TH1D("ref_cross_section","ref_cross_section",nbins,pt_bins.data());
     for(int i=0;i< nbins;i++)
     {
         ref_cross->SetBinContent(i+1,ref_cross_section_value[i]);
         ref_cross->SetBinError(i+1,error[i]);
     }
     return ref_cross;
 }