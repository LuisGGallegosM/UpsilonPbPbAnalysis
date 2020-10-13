#include "Main.h"

/**
 * @brief Does a invariant mass fit, from a branch in a tree
 * in a root file. Saves the plot in a tree and in pdf file format.
 * 
 * @param filename filename of the root file where to find the tree
 * @param outfilename filename of root file where to store fit results
 */
void massfit(const char* filename, const char* outfilename)
{
    TFile file(filename, "READ");

    TTree *tree_skimmed = (TTree *)file.Get(ONIATTREENAME);
    
    OniaMassFitter massFitter(tree_skimmed);

    TFile newfile(outfilename,"RECREATE");

    RooAbsReal* fittedFunc = massFitter.fit();

    TCanvas* canvas = drawData(massFitter.getVar(),massFitter.getDataset(),fittedFunc);

    newfile.cd();
    massFitter.getDataset()->Write();
    fittedFunc->Write();
    canvas->Write();
    canvas->SaveAs("files/massfit.pdf");
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