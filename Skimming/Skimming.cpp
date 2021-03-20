
#include <iostream>
#include <string>

#include "Skimming.h"

#if !defined(__CLING__)

int main(int argc, char **argv)
{
    if (argc == 4)
        OniaSkim(argv[1],argv[2],argv[3]);
    else if (argc ==5)
    {
        std::string flag= argv[1];
        if(flag=="-jet")
        {
            OniaJetSkim(argv[2],argv[3],argv[4]);
        }
        else
        {
            std::cerr << "Incorrect option flag\n";  
        }
    }
    else
    {
          std::cerr << "Incorrect number of parameters\n";  
    }
    return 0;
}

#endif