
#include "Main.h"

void Main()
{
    char filename[] ="files/merged_HiForestAOD.root";
    char outputfilename[] = "files/merged_HiForestAOD_skimmed.root";
    char fitfilename[] = "files/oniafit.root";

    kineCutParam kineCut;
    kineCut.ptLow=0.0f;
    kineCut.ptHigh=50.0f;
    kineCut.yLow=0.0f;
    kineCut.yHigh=2.4f;
    kineCut.massLow =8.5f;
    kineCut.massHigh =10.0f;
    kineCut.nBins = (kineCut.massHigh- kineCut.massLow)*20;

    //Skimming function
    Skim(filename,outputfilename);

    //Mass Fitting function
    massfit(outputfilename,fitfilename,&kineCut);

    //Generate drawings
    Drawing(fitfilename,&kineCut);

    return;
}

#if !defined(__CLING__)

int main(int argc, char **argv)
{
    Main();
    return 0;
}

#endif