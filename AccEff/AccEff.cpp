

#include "AccEff.h"
#include <string>
#include <iostream>

#if !defined(__CLING__)

int main(int argc, char **argv)
{
    std::string flags(argv[1]);

    switch (argc)
    {
        case 5:
        if (flags=="-eff")
            EffTest(argv[2],argv[3],argv[4],nullptr);
        else if (flags=="-acc")
            AccTest(argv[2],argv[3],argv[4]);
        else if (flags=="-acceff")
            AccEffCalculate(argv[2],argv[3],argv[4]);
        else
            std::cerr << "Incorrect number of parameters\n";
        break;

        case 6:
        if (flags=="-eff")
            EffTest(argv[2],argv[3],argv[4],argv[5]);
        else if (flags=="-addw")
            AddWeights(argv[2],argv[3],argv[4],argv[5]);
        else if (flags=="-addw2")
            AddWeights2(argv[2],argv[3],argv[4],argv[5]);
        else if(flags=="-crosssection")
            CrossSectionCalculate(argv[2],argv[3],argv[4],argv[5]);
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
        else if (flags=="-unfold")
            Unfold(argv[2],argv[3]);
        else if (flags=="-unfoldtrain1d")
            Unfold_Train1D(argv[2],argv[3]);
        else if (flags=="-unfold1d")
            Unfold1D(argv[2],argv[3]);
        else
            std::cerr << "Incorrect number of parameters\n";
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