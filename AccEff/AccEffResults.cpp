
#include "AccEffResults.h"
#include <iostream>
#include "TFile.h"
#include "TH1.h"
#include "TEfficiency.h"
#include "../Utils/utils.h"
#include "AccAnalyzer.h"
#include "EffAnalyzer.h"

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
    TFile* outFile = OpenFile(outputname,"CREATE");

    TH1F* acc_num= (TH1F*) accFile->Get(accNumName);
    TH1F* eff_num= (TH1F*) effFile->Get(effNumName);
    TH1F* acc_den= (TH1F*) accFile->Get(accDenName);
    TH1F* eff_den= (TH1F*) effFile->Get(effDenName);

    TH1F* dens= new TH1F((*acc_den)*(*eff_den));
    TH1F* nums= new TH1F((*acc_num)*(*eff_num));

    std::unique_ptr<TEfficiency> AccXEff = createTEff(nums,dens,"pt QQ AccXEff");

    std::string outbasename= ReplaceExtension(outputname,"");

    writeToCanvasEff(AccXEff.get(),"p_{T}","Acc x Eff",outbasename+"_AccXEff.pdf");
    
    TH1F* nSig = (TH1F*) fitFile->Get("nSigY1S");

    TH1F* nSigCorrectedNum = new TH1F((*nSig)*(*dens) );
    TH1F* nSigCorrectedDen = new TH1F((*nums));

    std::unique_ptr<TEfficiency> nSigCorrected = createTEff(nSigCorrectedNum,nSigCorrectedDen,"corrected raw yields");

    writeToCanvasEff(nSigCorrected.get(),"p_{T}","Corrected raw yields",outbasename+"_CorrectYields.pdf");

    AccXEff->Write();

    outFile->Close();
    accFile->Close();
    effFile->Close();
    fitFile->Close();
}