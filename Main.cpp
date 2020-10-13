
#include "Main.h"

void Main()
{
    char filename[] ="files/merged_HiForestAOD.root";
    char outputfilename[] = "files/merged_HiForestAOD_skimmed.root";
    char fitfilename[] = "files/oniafit.root";

    //Skimming function
    //Skim(filename,outputfilename);

    //Mass Fitting function
    massfit(outputfilename,fitfilename);

    return;
}

#if !defined(__CLING__)

int main(int argc, char **argv)
{
    Main();
    return 0;
}

#endif