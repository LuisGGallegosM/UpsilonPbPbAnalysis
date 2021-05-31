
#include"ClosureTest.h"

#include "../Common/AccEffAux.h"
#include"../../Utils/Helpers/Helpers.h"
#include "../../Drawing/Drawing.h"

#include "../Acc/AccAnalyzer.h"
#include "../Eff/EffAnalyzer.h"

#include<iostream>

#include"TEfficiency.h"
#include"TH1.h"

void accOnlyClosureTest(const TH1D* yields, const TEfficiency* acc, const TH1D* yields_original, const std::string& outputname );
void effOnlyClosureTest(const TH1D* yields, const TEfficiency* eff,const TH1D* yields_original, const std::string& outputname);
void acceffClosureTest(TH1D* MC_gen_yields, TH1D* MC_reco_yields, TEfficiency* accXEff, const std::string& outputname);

void ClosureTest(const char* accxeffFilename,const char* accFilename, const char* effFilename, const char* accSampleFilename, const char* effSampleFilename, const char* effCutParams, const char* outputname)
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
    std::cout << "Reading Acc sample file: " << accSampleFilename <<'\n';
    TFile* accSampleFile = OpenFile(accSampleFilename,"READ");

    //input file
    std::cout << "Reading Eff sample file: " << effSampleFilename <<'\n';
    TFile* effSampleFile = OpenFile(effSampleFilename,"READ");

    //output file
    std::cout << "Writing to file: " << outputname <<'\n';
    TFile* outFile = OpenFile(outputname,"RECREATE");

    TEfficiency* accXEff = (TEfficiency*) accXEffFile->Get(accXEffName);
    TEfficiency* acc = (TEfficiency*) accFile->Get(accName);
    TEfficiency* eff = (TEfficiency*) effFile->Get(effName);

    std::string outputbasename=ReplaceExtension(outputname,"");

    WeightFuncTEff weightAcc(acc);
    WeightFuncTEff weightEff(eff);

    TTree* accSampleTree= GetTree(accSampleFile,"hionia/myTree");

    AccAnalyzer accAnalyzer(accSampleTree,"DetectableOnia", &weightAcc, true );
    accAnalyzer.Test(outputbasename+"_Acc");
    const AccHistografer* accHist = accAnalyzer.getHists();

    accOnlyClosureTest(accHist->ptHistQQDet,acc,accHist->ptHistQQGen, outputbasename);

    TTree* effSampleTree= GetTree(effSampleFile,"hionia/myTree");

    CutParams cut;
    cut.deserialize(effCutParams);

    EffAnalyzerMC effAnalyzer(effSampleTree,&cut,"DetectableOnia", &weightEff, true );
    effAnalyzer.Test(outputbasename+"_Eff");
    const EffHistografer* effHist = effAnalyzer.getHists();

    effOnlyClosureTest(effHist->ptHistQQRecoCut,eff,effHist->ptHistQQDet,outputbasename);

    outFile->Close();
    accFile->Close();
    accXEffFile->Close();
    
}

void accOnlyClosureTest(const TH1D* yields, const TEfficiency* acc, const TH1D* yields_original, const std::string& outputname )
{
    TH1D yield_ratio = (*yields)/(*yields_original);
    yield_ratio.SetName("MC_gen_acc_corr_ratio_closure");
    yield_ratio.SetTitle("acceptance closure test");
    writeToCanvas(&yield_ratio,"p^{#mu#mu}_{T} GeV/c", "ratio",outputname);
}

void effOnlyClosureTest(const TH1D* yields, const TEfficiency* eff,const TH1D* yields_original, const std::string& outputname)
{
    TH1D yield_ratio = (*yields)/(*yields_original);
    yield_ratio.SetName("MC_gen_eff_corr_ratio_closure");
    yield_ratio.SetTitle("efficiency closure test");
    writeToCanvas(&yield_ratio,"p^{#mu#mu}_{T} GeV/c", "ratio",outputname);
}

void acceffClosureTest(TH1D* MC_gen_yields, TH1D* MC_reco_yields, TEfficiency* accXEff, const std::string& outputname)
{
    MC_gen_yields->SetLineColor(3);
    MC_gen_yields->SetLineWidth(2);
    MC_gen_yields->Scale( 1.0f/MC_gen_yields->Integral() );
    MC_gen_yields->SetTitle("MC generated normalized");

    TH1D* MC_reco_yields_corr = calcCorrectedYields( MC_reco_yields,accXEff );
    MC_reco_yields_corr->Scale( 1.0f/MC_reco_yields_corr->Integral());
    MC_reco_yields_corr->SetTitle("MC corrected yields normalized");

    std::vector<TH1*> vec={MC_reco_yields_corr, MC_gen_yields };
    writeToCanvas(vec, "MC reco vs Gen" , "p^{#mu#mu}_{T} GeV/c","N_{Y1Scorr}",outputname+".pdf",false);

    TH1D* MC_reco_gen_ratio= new TH1D((*MC_reco_yields_corr)/(*MC_gen_yields));
    MC_reco_gen_ratio->SetName("MC_reco_gen_ratio");
    MC_reco_gen_ratio->SetTitle("#frac{MCreco corr}{MCgen}");
    writeToCanvas(MC_reco_gen_ratio,"p^{#mu#mu}_{T} GeV/c","nSigY1S",outputname);

    MC_reco_gen_ratio->Write();
    MC_reco_yields_corr->Write();

}