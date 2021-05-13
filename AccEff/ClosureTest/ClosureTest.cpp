
#include"ClosureTest.h"

#include "../Common/AccEffAux.h"
#include"../../Utils/Helpers/Helpers.h"
#include "../../Drawing/Drawing.h"

#include<iostream>

#include"TEfficiency.h"
#include"TH1.h"

void acceffClosureTest(TH1F* MC_gen_yields, TH1F* MC_reco_yields, TEfficiency* accXEff, const std::string& outputname);
void accOnlyClosureTest(TH1F* MC_gen_det_yields, TEfficiency* acc, TH1F* MC_gen_yields, const std::string& outputname);
void effOnlyClosureTest(TH1F* yields, TEfficiency* eff, TH1F* yields_original, const std::string& outputname);

void ClosureTest(const char* accxeffFilename,const char* accFilename, const char* effFilename, const char* fitFilename, const char* outputname)
{
    std::cout << "\nCLOSURE TEST\n";

    //input file
    std::cout << "Reading AccXEff file: " << accxeffFilename <<'\n';
    TFile* accXEffFile = OpenFile(accxeffFilename,"READ");

    //input file
    std::cout << "Reading Acc file: " << accFilename <<'\n';
    TFile* accFile = OpenFile(accFilename,"READ");

    //input file
    std::cout << "Reading Eff file: " << effFilename <<'\n';
    TFile* effFile = OpenFile(effFilename,"READ");

    //input file
    std::cout << "Reading MC fit file: " << fitFilename <<'\n';
    TFile* fitFile = OpenFile(fitFilename,"READ");

    //output file
    std::cout << "Writing to file: " << outputname <<'\n';
    TFile* outFile = OpenFile(outputname,"RECREATE");

    TEfficiency* accXEff = (TEfficiency*) accXEffFile->Get(accXEffName);
    TEfficiency* acc = (TEfficiency*) accFile->Get(accName);
    TEfficiency* eff = (TEfficiency*) effFile->Get(effName);

    TH1F* acc_den = (TH1F*) accFile->Get(accDenName);
    TH1F* acc_num = (TH1F*) accFile->Get(accNumName);
    TH1F* eff_den = (TH1F*) effFile->Get(effDenName);
    TH1F* eff_num = (TH1F*) effFile->Get(effNumName);
    TH1F* MC_reco_yields = (TH1F*) fitFile->Get(nSigY1SName);

    accOnlyClosureTest(acc_num,acc,acc_den,outputname);

    effOnlyClosureTest(eff_num,eff,eff_den,outputname);

    acceffClosureTest(acc_den,MC_reco_yields,accXEff,outputname);
    
    acc_den->Write();

    outFile->Close();
    accFile->Close();
    fitFile->Close();
    accXEffFile->Close();
    
}

void accOnlyClosureTest(TH1F* yields, TEfficiency* acc, TH1F* yields_original, const std::string& outputname )
{
    TH1F* yields_acccorr = calcCorrectedYields(yields,acc,"_acccorr");
    TH1F yield_ratio = (*yields_acccorr)/(*yields_original);
    yield_ratio.SetName("MC_gen_acc_corr_ratio_closure");
    yield_ratio.SetTitle("acceptance closure test");
    writeToCanvas(&yield_ratio,"p^{#mu#mu}_{T} GeV/c", "ratio",outputname);
}

void effOnlyClosureTest(TH1F* yields, TEfficiency* eff, TH1F* yields_original, const std::string& outputname)
{
    TH1F* yields_effcorr = calcCorrectedYields(yields,eff,"_effcorr");
    TH1F yield_ratio = (*yields_effcorr)/(*yields_original);
    yield_ratio.SetName("MC_gen_eff_corr_ratio_closure");
    yield_ratio.SetTitle("efficiency closure test");
    writeToCanvas(&yield_ratio,"p^{#mu#mu}_{T} GeV/c", "ratio",outputname);
}

void acceffClosureTest(TH1F* MC_gen_yields, TH1F* MC_reco_yields, TEfficiency* accXEff, const std::string& outputname)
{
    MC_gen_yields->SetLineColor(3);
    MC_gen_yields->SetLineWidth(2);

    TH1F* MC_reco_yields_corr = calcCorrectedYields( MC_reco_yields,accXEff );

    MC_gen_yields->Scale( 1.0f/MC_gen_yields->Integral() );
    MC_reco_yields_corr->Scale( 1.0f/MC_reco_yields_corr->Integral());
    MC_gen_yields->SetTitle("MC generated normalized");
    MC_reco_yields_corr->SetTitle("MC corrected yields normalized");

    std::vector<TH1*> vec={MC_reco_yields_corr, MC_gen_yields };
    writeToCanvas(vec, "MC reco vs Gen" , "p^{#mu#mu}_{T} GeV/c","N_{Y1Scorr}",ReplaceExtension(outputname.data(),".pdf"),false);

    TH1F* MC_reco_gen_ratio= new TH1F((*MC_reco_yields_corr)/(*MC_gen_yields));
    MC_reco_gen_ratio->SetName("MC_reco_gen_ratio");
    MC_reco_gen_ratio->SetTitle("#frac{MCreco corr}{MCgen}");
    writeToCanvas(MC_reco_gen_ratio,"p^{#mu#mu}_{T} GeV/c","nSigY1S",outputname);

    MC_reco_gen_ratio->Write();
    MC_reco_yields_corr->Write();

}