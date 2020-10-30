#include "Main.h"

/**
 * @brief Does a invariant mass fit, from a branch in a tree
 * in a root file. Saves the plot in a tree and in pdf file format.
 * 
 * @param filename filename of the root file where to find the tree
 * @param outfilename filename of root file where to store fit results
 */
void massfit(const char* filename, const char* outfilename, const fitConfig* config)
{
    TFile file(filename, "READ");

    TTree *tree_skimmed = (TTree *)file.Get(ONIATTREENAME);
    
    OniaMassFitter massFitter(tree_skimmed, config);

    TFile newfile(outfilename,"RECREATE");

    //redirect cout
    std::string logFilename(outfilename);
    logFilename +=".log";
    std::ofstream logFile((logFilename).data());
    std::cout.rdbuf(logFile.rdbuf());

    RooAbsReal* fittedFunc = massFitter.fit();

    //save cut file
    std::string copyFilename(outfilename);
    copyFilename +=".txt";
    std::ofstream copyFile(copyFilename.data());

    std::string cutFilename(filename);
    cutFilename +=".txt";
    std::ifstream cutFile(cutFilename.data());

    copyFile << cutFile.rdbuf();

    massFitter.getResults()->Print();

    newfile.cd();
    massFitter.getDataset()->Write();
    massFitter.getResults()->Write("fitresults");
    fittedFunc->Write();
    massFitter.getVar()->Write();
}

