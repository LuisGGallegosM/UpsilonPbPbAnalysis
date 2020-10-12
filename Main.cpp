
#include "Main.h"

void massfit(const char* filename);
void drawData(RooRealVar* var, RooDataSet* dataset, RooAbsReal* fittedFunc);

void Main()
{
    char filename[] ="files/merged_HiForestAOD.root";
    char outputfilename[] = "files/merged_HiForestAOD_skimmed.root";

    //Skimming function
    //Skim(filename,outputfilename);

    //Mass Fitting function
    massfit(outputfilename);

    return;
}

/**
 * @brief Does a invariant mass fit, from a branch in a tree
 * in a root file. Saves the plot in a tree and in pdf file format.
 * 
 * @param filename filename of the root file where to find the tree
 */
void massfit(const char* filename)
{
    TFile file(filename, "READ");

    TTree *tree_skimmed = (TTree *)file.Get(ONIATTREENAME);
    
    OniaMassFitter massFitter(tree_skimmed);

    TFile newfile("files/oniafit.root","RECREATE");

    RooAbsReal* fittedFunc = massFitter.fit();

    drawData(massFitter.getVar(),massFitter.getDataset(),fittedFunc);
}

/**
 * @brief Draws the data in variable var of dataset and fittedFunc in same plot.
 * 
 * @param var variable to use in x axis.
 * @param dataset experimental/sim data points.
 * @param fittedFunc fitted curve to experimental/sim
 */
void drawData(RooRealVar* var, RooDataSet* dataset, RooAbsReal* fittedFunc)
{
    TCanvas canvas("My plot","mass fit");
    canvas.cd();

    RooPlot* plot = var->frame();

    //draw fitted function
    fittedFunc->plotOn(plot,Normalization(1.0,RooAbsReal::RelativeExpected));

    //draw dataset
    dataset->plotOn(plot);

    //draw and save the canvas in root file and pdf format
    plot->Draw();
    canvas.Write();
    canvas.SaveAs("files/massfit.pdf");
}

#if !defined(__CLING__)

int main(int argc, char **argv)
{
    Main();
    return 0;
}

#endif