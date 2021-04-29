

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
            EffTest(argv[2],argv[3],argv[4],false);
        else if (flags=="-effcorr")
            EffTest(argv[2],argv[3],argv[4],true);
        else if (flags=="-acc")
            AccTest(argv[2],argv[3],argv[4]);
        else
            std::cerr << "Incorrect number of parameters\n";
        break;

        case 6:
        if (flags=="-closure")
            ClosureTest(argv[2],argv[3],argv[4],argv[5]);
        else
            std::cerr << "Incorrect number of parameters\n";
        break;

        case 4:
        if(flags=="-acc")
            AccTest(argv[2],argv[3],nullptr);
        else if (flags=="-fit")
            YieldFit(argv[2],argv[3]);
        else
            std::cerr << "Incorrect number of parameters\n";
        break;

        case 7:
        if (flags=="-final")
            AccEffResults(argv[2],argv[3],argv[4],argv[5],argv[6]);
        break;

        default:
        std::cerr << "Incorrect number of parameters\n";
        break;

    }
    return 0;
}

#endif