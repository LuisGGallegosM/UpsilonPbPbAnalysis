

#include "AccEff.h"
#include <string>
#include <iostream>

#if !defined(__CLING__)

int main(int argc, char **argv)
{
    std::string flags(argv[1]);
    if ( (argc == 5) || (argc ==4) )
    {
        if (flags=="-eff")
            EffTest(argv[2],argv[3],argv[4]);
    }
    else if (argc == 4)
    {
        if(flags=="-acc")
            AccTest(argv[2],argv[3]);
    }
    else if (argc == 6)
    {
        if (flags=="-final")
            AccEffResults(argv[2],argv[3],argv[4],argv[5]);
    }
    else
    {
        std::cerr << "Incorrect number of parameters\n";
    }
    return 0;
}

#endif