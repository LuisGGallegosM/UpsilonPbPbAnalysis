#include "Main.h"

/**
 * @brief Does a invariant mass fit, from a branch in a tree
 * in a root file. Saves the plot in a tree and in pdf file format.
 * 
 * @param filename filename of the root file where to find the tree
 * @param outfilename filename of root file where to store fit results
 */
void massfit(const char* filename, const char* outfilename, const kineCutParam* kineCut)
{
    TFile file(filename, "READ");

    TTree *tree_skimmed = (TTree *)file.Get(ONIATTREENAME);
    
    OniaMassFitter massFitter(tree_skimmed, kineCut->massLow, kineCut->massHigh);

    TFile newfile(outfilename,"RECREATE");

    RooAbsReal* fittedFunc = massFitter.fit();
    
    newfile.cd();
    massFitter.getDataset()->Write();
    massFitter.getResults()->Write("fitresults");
    fittedFunc->Write();
    massFitter.getVar()->Write();
}

