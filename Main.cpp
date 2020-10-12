
#include "Main.h"

void massfit(const char* filename);

void Main()
{
    char filename[] ="files/merged_HiForestAOD.root";
    char outputfilename[] = "files/merged_HiForestAOD_skimmed.root";

    //Skim(filename,outputfilename);

    massfit(outputfilename);

    return;
}

void massfit(const char* filename)
{
    TFile file(filename, "READ");
    TTree *tree_skimmed = (TTree *)file.Get(oniaTTreeName);
    
    OniaMassFitter massFitter(tree_skimmed);

    TFile newfile("files/oniafit.root","RECREATE");

    RooAbsReal* fittedFunc = massFitter.fit();

    TCanvas canvas("My plot","mass fit");
    canvas.cd();

    RooPlot* plot = massFitter.getVar()->frame();
    fittedFunc->plotOn(plot,Normalization(1.0,RooAbsReal::RelativeExpected));
    massFitter.getDataset()->plotOn(plot);
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