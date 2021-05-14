
#include"YieldFit.h"

#include<iostream>

#include"../../Utils/Helpers/Helpers.h"
#include"../../Drawing/Drawing.h"
#include"../Common/AccEffAux.h"
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

    TH1F* ratios = (TH1F*) ratiosFile->Get(yieldFitName);

    YieldFitter fitter(ratios);
    RooAbsReal* fitFunc = fitter.fit();

    ParameterGroup fittedParams = fitter.getParams();
    fittedParams.serialize( ReplaceExtension(outputFile,".fit"));

    outFile->cd();
    fitFunc->Write();
    fitter.getHist()->Write();
    plotFit(fitFunc,fitter.getHist(),fitter.getVar(), ReplaceExtension(outputFile,".pdf"));

    outFile->Close();
    ratiosFile->Close();

}

RooPlot* plotFit(RooAbsReal* fitFunc,RooDataHist* data, RooRealVar* var,const std::string& outname)
{
    TCanvas* canvas= new TCanvas("yield_fit","fitting of yield ratios", canvasWidth ,canvasHeight);
    canvas->cd();
    TPad* pad = new TPad("pad","fit", padSizes[0], padSizes[1], padSizes[2], padSizes[3]);;
    pad->Draw();
    pad->cd();
    RooPlot* plot = var->frame();
    fitFunc->plotOn(plot,Name("fit Func"),LineColor(kOrange+7));
    data->plotOn(plot,Name("data"),LineColor(kBlue));
    plot->SetAxisRange(0.0,3.0,"Y");
    plot->SetYTitle("ratio");
    plot->SetTitle("Fit of ratio of yields");
    plot->Draw();
    canvas->Write();
    canvas->SaveAs(outname.data());
    return plot;
}