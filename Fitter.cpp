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

    RooPlot* plot = var->frame();

    //draw fitted function
    fittedFunc->plotOn(plot,Normalization(1.0,RooAbsReal::RelativeExpected));

    //draw dataset
    dataset->plotOn(plot);

    //RooHist* pullHist = plot->pullHist(dataset->GetName(),fittedFunc->GetName());

    //draw and save the canvas in root file and pdf format
    plot->Draw();
    return canvas;
}