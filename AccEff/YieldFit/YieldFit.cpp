
#include"YieldFit.h"

#include<iostream>

#include"../../Utils/Helpers/Helpers.h"
#include"YieldFitter.h"

#include"RooRealVar.h"
#include"RooGenericPdf.h"
#include"RooPlot.h"
#include"TCanvas.h"

using namespace RooFit;

RooPlot* plotFit(RooAbsReal* fitFunc,RooDataHist* data, RooRealVar* var,const std::string& outname);

void YieldFit(const char* inputRatiosFile, const char* outputFile)
{
    std::cout << "\nYIELD RATIOS FITTING \n";

    //input file
    std::cout << "Reading ratios input file: " << inputRatiosFile <<'\n';
    TFile* ratiosFile = OpenFile(inputRatiosFile,"READ");

    //output file
    std::cout << "Writing to file: " << outputFile <<'\n';
    TFile* outFile = OpenFile(outputFile,"RECREATE");

    TH1F* ratios = (TH1F*) ratiosFile->Get("ratio_dN_dpT_norm");

    YieldFitter fitter(ratios);
    RooAbsReal* fitFunc = fitter.fit();

    outFile->cd();
    fitFunc->Write();
    fitter.getHist()->Write();
    plotFit(fitFunc,fitter.getHist(),fitter.getVar(), ReplaceExtension(outputFile,".pdf"));

    outFile->Close();
    ratiosFile->Close();

}

RooPlot* plotFit(RooAbsReal* fitFunc,RooDataHist* data, RooRealVar* var,const std::string& outname)
{
    TCanvas* canvas= new TCanvas("yield_fit","fitting of yield ratios",4,45,600,600);
    canvas->cd();
    TPad* pad = new TPad("pad","fit", 0.08, 0.08, 0.92, 0.92);
    pad->Draw();
    pad->cd();
    RooPlot* plot = var->frame();
    fitFunc->plotOn(plot,Name("fit Func"),LineColor(kOrange+7));
    data->plotOn(plot,Name("data"),LineColor(kBlue));
    plot->SetAxisRange(0.0,3.0,"Y");
    plot->Draw();
    canvas->Write();
    canvas->SaveAs(outname.data());
    return plot;
}