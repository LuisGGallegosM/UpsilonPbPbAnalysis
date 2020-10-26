
#include "Main.h"

using namespace std;

void Main()
{
    string data_filename ="files/merged_HiForestAOD.root";
    string skimmed_filename = "files/merged_HiForestAOD_skimmed.root";
    string folder_name   = "files/fit_exlimit";

    string fit_filename = folder_name + "/oniafit.root";
    string drawing_filename = folder_name + "/oniafit.pdf";
  
    kineCutParam kineCut;
    kineCut.ptLow=0.0f;
    kineCut.ptHigh=50.0f;
    kineCut.yLow=0.0f;
    kineCut.yHigh=2.4f;
    kineCut.massLow =8.5f;
    kineCut.massHigh =10.0f;
    kineCut.singleMuPtLow=3.5f;
    kineCut.singleMuEtaHigh=2.4f;
    kineCut.nBins = (kineCut.massHigh- kineCut.massLow)*800;
    kineCut.bkgOn =true;

    gSystem->mkdir(folder_name.data());

    //Skimming function
    //Skim(data_filename.data(),skimmed_filename.data());

    //Mass Fitting function
    //massfit(skimmed_filename.data(),fit_filename.data(),&kineCut);

    //Generate drawings
    Drawing(fit_filename.data(),drawing_filename.data(),&kineCut);

    return;
}

#if !defined(__CLING__)

int main(int argc, char **argv)
{
    Main();
    return 0;
}

#endif