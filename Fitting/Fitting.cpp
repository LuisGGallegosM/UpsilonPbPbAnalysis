

#include <string>
#include <iostream>

void Fit(const char* filename, const char* cutfilename, const char* outfilename, const char* fitconfigname);
void DrawCmp(const char* xvar, const char* outputpath,int size,const char** fitfilepaths);
void DrawPlot(const char* inputdirectoryname, const char* configfilename  );
#include"MultifitGen/MultifitGen.h"

#if !defined(__CLING__)

int main(int argc, char **argv)
{
    std::string option(argv[1]);

    if (option=="-multidraw")
    {
        //enter in multicomparation draw mode
        int numFiles=(argc -4) ;
        const char* args[64];

        if (numFiles<=0) return 0;
        
        for (int i=0;i<numFiles;i++) args[i]=argv[i+4];

        DrawCmp(argv[2],argv[3],numFiles,args);
    }
    else if (option=="-draw")
    {
        //normal mode draw
        if (argc ==4)
            DrawPlot(argv[2],argv[3]);
        else
        {
            std::cerr << "Error: Incorrect number of parameters\n";  
        }
    }
    else if (option=="-fit")
    {
        if (argc ==6)
        {
            Fit(argv[2],argv[3],argv[4],argv[5]);
        }
        else
        {
            std::cerr << "Incorrect number of parameters\n";  
        }
    }
    else if (option=="-multifitgen")
    {
        if (argc ==4)
        {
            MultifitGen(argv[2],argv[3]);
        }
        else
        {
            std::cerr << "Incorrect number of parameters\n";  
        }
    }
    else
    {
        std::cerr << "Incorrect option flags\n";  
    }

        
    return 0;
}

#endif