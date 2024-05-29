

#include "AccEff.h"
#include <string>
#include <iostream>

#if !defined(__CLING__)

int main(int argc, char **argv)
{
    std::string flags(argv[1]);

    if(flags=="-yagg")
    {
        const int filesnum=argc-3;
        const char* files[10];

        for(int i=0;i<filesnum;i++)
        files[i]= argv[i+2];

        YieldAgg(files,filesnum,argv[argc-1]);
        return 0;
    }

    switch (argc)
    {
        case 3:
        if(flags=="-syst")
        {
            systgraph(argv[2]);
        }
        break;
        case 5:
        if (flags=="-eff")
            EffTest(argv[2],argv[3],argv[4],nullptr);
        else if (flags=="-acc")
            AccTest(argv[2],argv[3],argv[4]);
        else if (flags=="-acceff")
            AccEffCalculate(argv[2],argv[3],argv[4]);
        else if (flags=="-unfold")
            Unfold(argv[2],argv[3],argv[4]);
        else
            std::cerr << "Incorrect number of parameters\n";
        break;

        case 6:
        if (flags=="-eff")
            EffTest(argv[2],argv[3],argv[4],argv[5]);
        else if (flags=="-addw")
            AddWeights(argv[2],argv[3],argv[4],argv[5],false);
        else if (flags=="-addw2")
            AddWeights(argv[2],argv[3],argv[4],argv[5],true);
        else if(flags=="-crosssection")
            CrossSectionCalculate(argv[2],argv[3],argv[4],argv[5]);
        else if (flags=="-toys")
            generateToys(argv[2],argv[3],argv[4],argv[5]);
        else if (flags=="-systoycnt")
            acceff_systematics(argv[2],argv[3],argv[4],argv[5]);
        else
            std::cerr << "Incorrect number of parameters\n";
        break;

        case 4:
        if(flags=="-acc")
            AccTest(argv[2],argv[3],nullptr);
        else if (flags=="-fit")
            YieldFit(argv[2],argv[3]);
        else if (flags=="-unfoldtrain")
            Unfold_Train(argv[2],argv[3]);
        else if (flags=="-unfoldtest")
            Unfold_Test(argv[2],argv[3]);
        else if (flags=="-unfoldtrain1d")
            Unfold_Train1D(argv[2],argv[3]);
        else if (flags=="-unfold1d")
            Unfold1D(argv[2],argv[3]);
        else if(flags=="-efffinebin")
            EffFineBinning(argv[2],argv[3]);
        else if (flags=="-accfinebin")
            AccFineBinning(argv[2],argv[3]);
        else if (flags=="-jecan")
        {
            jecan(argv[2],argv[3]);
        } else if(flags=="-unfold_syst")
            unfolding_create_systematic_vars(argv[2],argv[3]);
        else
        {
            std::cerr << "Incorrect number of parameters\n";
        }
            
        break;

        case 8:
        if (flags=="-closurejet")
            ClosureTestJet(argv[2],argv[3],argv[4],argv[5],argv[6],argv[7]);
        else
            std::cerr << "Incorrect number of parameters\n";
        break;

        case 9:
        if (flags=="-closure")
            ClosureTest(argv[2],argv[3],argv[4],argv[5],argv[6],argv[7],argv[8]);
        else
            std::cerr << "Incorrect number of parameters\n";
        break;

        default:
        std::cerr << "Incorrect number of parameters\n";
        break;

    }
    return 0;
}

#endif