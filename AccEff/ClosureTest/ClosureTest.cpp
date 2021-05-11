
#include"ClosureTest.h"

#include "../Common/AccEffAux.h"
#include"../../Utils/Helpers/Helpers.h"
#include "../../Drawing/Drawing.h"

#include<iostream>

#include"TEfficiency.h"
#include"TH1.h"

void ClosureTest(const char* accxeffFilename,const char* accFilename, const char* fitFilename, const char* outputname)
{
    std::cout << "\nCLOSURE TEST\n";

    //input file
    std::cout << "Reading AccXEff file: " << accxeffFilename <<'\n';
    TFile* accXEffFile = OpenFile(accxeffFilename,"READ");

    //input file
    std::cout << "Reading Acc file: " << accFilename <<'\n';
    TFile* accFile = OpenFile(accFilename,"READ");

    //input file
    std::cout << "Reading MC fit file: " << fitFilename <<'\n';
    TFile* fitFile = OpenFile(fitFilename,"READ");

    //output file
    std::cout << "Writing to file: " << outputname <<'\n';
    TFile* outFile = OpenFile(outputname,"RECREATE");

    TEfficiency* accXEff = (TEfficiency*) accXEffFile->Get(accXEffName);
    TEfficiency* acc = (TEfficiency*) accFile->Get(accName);

    TH1F* MC_gen_yields = (TH1F*) accFile->Get(accDenName);
    TH1F* MC_gen_det_yields = (TH1F*) accFile->Get(accNumName);
    TH1F* MC_reco_yields = (TH1F*) fitFile->Get(nSigY1SName);

    MC_gen_yields->SetLineColor(3);
    MC_gen_yields->SetLineWidth(2);

    TH1F* MC_reco_yields_corr = calcCorrectedYields( MC_reco_yields,accXEff );
    TH1F* MC_genonly_yields_acccorr = calcCorrectedYields(MC_gen_det_yields,acc,"_acccorr");

    TH1F MC_genonly_yield_corr_acc_ratio = (*MC_genonly_yields_acccorr)/(*MC_gen_yields);
    MC_genonly_yield_corr_acc_ratio.SetName("MC_gen_acc_corr_ratio_closure");
    writeToCanvas(&MC_genonly_yield_corr_acc_ratio,"p^{#mu#mu}_{T} GeV/c", "ratio",outputname);

    MC_gen_yields->Scale( 1.0f/MC_gen_yields->Integral() );
    MC_reco_yields_corr->Scale( 1.0f/MC_reco_yields_corr->Integral());
    MC_gen_yields->SetTitle("MC generated normalized");
    MC_reco_yields_corr->SetTitle("MC corrected yields normalized");

    std::vector<TH1*> vec={MC_reco_yields_corr, MC_gen_yields };
    writeToCanvas(vec, "MC reco vs Gen" , "p^{#mu#mu}_{T} GeV/c","N_{Y1Scorr}",ReplaceExtension(outputname,".pdf"),false);

    TH1F* MC_reco_gen_ratio= new TH1F((*MC_reco_yields_corr)/(*MC_gen_yields));
    MC_reco_gen_ratio->SetName("MC_reco_gen_ratio");
    MC_reco_gen_ratio->SetTitle("#frac{MCreco corr}{MCgen}");
    writeToCanvas(MC_reco_gen_ratio,"p^{#mu#mu}_{T} GeV/c","nSigY1S",outputname);

    MC_reco_gen_ratio->Write();
    MC_reco_yields_corr->Write();
    MC_gen_yields->Write();

    outFile->Close();
    accFile->Close();
    fitFile->Close();
    accXEffFile->Close();
    
}