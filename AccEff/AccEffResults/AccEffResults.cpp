
#include "AccEffResults.h"
#include <iostream>

#include "TFile.h"
#include "TH1.h"
#include "TEfficiency.h"
#include "TGraphAsymmErrors.h"

#include "../Common/AccEffAux.h"
#include "../../Utils/Helpers/Helpers.h"
#include "../../Drawing/Drawing.h"

const char xAxisLabel[] ="p^{#mu#mu}_{T} GeV/c";

void AccEffCalculate(const char* accFilename, const char* effFilename, const char* outputname)
{
    std::cout << "\nACCEPTANCY EFFICIENCY RESULT TEST\n";

    //input file
    std::cout << "Reading acceptancy input file: " << accFilename <<'\n';
    TFile* accFile = OpenFile(accFilename,"READ");
    //input file
    std::cout << "Reading effiency input file: " << effFilename <<'\n';
    TFile* effFile = OpenFile(effFilename,"READ");
    //output file
    std::cout << "Writing to file: " << outputname <<'\n';
    TFile* outFile = OpenFile(outputname,"RECREATE");

    std::string outbasename= ReplaceExtension(outputname,"");

    TH1D* acc_num= (TH1D*) accFile->Get(accNumName);
    TH1D* eff_num= (TH1D*) effFile->Get(effNumName);
    TH1D* acc_den= (TH1D*) accFile->Get(accDenName);
    TH1D* eff_den= (TH1D*) effFile->Get(effDenName);

    TH1D* dens= new TH1D((*acc_den)*(*eff_den));
    TH1D* nums= new TH1D((*acc_num)*(*eff_num));

    std::unique_ptr<TEfficiency> AccXEff = createTEff(nums,dens,accXEffName,"#alpha #epsilon;"+ std::string(xAxisLabel)+"; #alpha#epsilon;");
    AccXEff->Write();
    writeToCanvasEff(AccXEff.get(),"p_{T}","Acc x Eff",outbasename);

    outFile->Close();
    accFile->Close();
    effFile->Close();
}