
#include "Main.h"

using namespace std;
using namespace RooFit;

TCanvas* drawData(RooRealVar* var, RooDataSet* dataset, RooAbsReal* fittedFunc);

void Drawing(const char* filename)
{
    TFile file(filename,"UPDATE");

    if (file.IsZombie())
    {
        cout << "File " << filename << " not found.";
        return;
    }

    RooRealVar* massVar = (RooRealVar*) (file.Get("mass"));
    RooAbsReal* fittedFunc = (RooAbsReal*) (file.Get("dcb_fit"));
    RooDataSet* dataset = (RooDataSet*) (file.Get("dataset"));

    if ((massVar==nullptr) || (fittedFunc==nullptr) || (dataset==nullptr))
    {
        cout << "Needed data not found in file" ;
        return;
    }

    TCanvas* canvas = drawData(massVar,dataset,fittedFunc);

    if (canvas==nullptr)
    {
        cout << "Canvas not generated";
        return;
    }

    canvas->Write(0,TObject::kOverwrite);
    canvas->SaveAs("files/massfit.pdf");

    return;
}

/**
 * @brief Draws the data in variable var of dataset and fittedFunc in same plot.
 * 
 * @param var variable to use in x axis.
 * @param dataset experimental/sim data points.
 * @param fittedFunc fitted curve to experimental/sim
 * @return Drawn canvas.
 */
TCanvas* drawData(RooRealVar* var, RooDataSet* dataset, RooAbsReal* fittedFunc)
{
    TCanvas* canvas = new TCanvas("Fit_plot","mass fit");
    canvas->cd();

    TPad* graph = new TPad("graph_pad","mass fit",0.0,0.2,1.0,1.0);
    TPad* pull = new TPad("pull_pad","mass fit pull",0.0,0.0,1.0,0.2);

    graph->Draw();
    pull->Draw();

    graph->cd();
    //draw fitted and dataset function
    RooPlot* plot = var->frame();
    fittedFunc->plotOn(plot,Name("fitFunc"),Normalization(1.0,RooAbsReal::RelativeExpected));
    dataset->plotOn(plot,Name("dataset"));

    plot->SetTitle("Upsilon (1S) MC");
    plot->GetXaxis()->SetTitleSize(0);
    plot->Draw();

    //fittedFunc->getParameters(dataset)->printLatex();

    //draw pull
    pull->cd();
    RooPlot* pullPlot = var->frame();
    RooHist* pullHist = plot->pullHist("dataset","fitFunc");
    pullPlot->addPlotable(pullHist,"P");
    pullPlot->GetYaxis()->SetTitle("Pull");
    pullPlot->GetYaxis()->SetLabelSize(0.13) ;

    pullPlot->GetXaxis()->SetTitle("m_{#mu^{+}#mu^{-}} (GeV/c^{2})");
    
    pullPlot->Draw();

    return canvas;
}