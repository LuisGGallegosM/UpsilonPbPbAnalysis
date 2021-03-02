
#include "AccEffResults.h"
#include <iostream>
#include "TFile.h"
#include "TH1.h"
#include "TEfficiency.h"
#include "../Utils/utils.h"

void AccEffResults(const char* accFilename, const char* effFilename, const char* fitfilepath, const char* outputname)
{
    std::cout << "\nACCEPTANCY EFFICIENCY RESULT TEST\n";

    //input file
    std::cout << "Reading acceptancy input file: " << accFilename <<'\n';
    TFile* accFile = OpenFile(accFilename,"READ");
    //input file
    std::cout << "Reading effiency input file: " << effFilename <<'\n';
    TFile* effFile = OpenFile(effFilename,"READ");
    //input file
    std::cout << "Reading multifit file: " << fitfilepath <<'\n';
    TFile* fitFile = OpenFile(fitfilepath,"READ");
    //reading file
    std::cout << "Writing to file: " << outputname <<'\n';
    TFile* outFile = OpenFile(outputname,"CREATE");

    TEfficiency* acceptancy = (TEfficiency*) accFile->Get("pt QQ Acceptancy");
    TEfficiency* efficiency = (TEfficiency*) effFile->Get("pt QQ Efficiency");

    TH2* acc = acceptancy->CreateHistogram();

    //TH1F AccXEff = ((*acceptancy)*(*efficiency));

    //AccXEff.Write();

    outFile->Close();
    accFile->Close();
    effFile->Close();
    fitFile->Close();
}