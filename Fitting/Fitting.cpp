

#include <string>
#include <iostream>

void Fit(const char* filename, const char* cutfilename, const char* outfilename, const char* fitconfigname);
void DrawCmp(const char* xvar, const char* outputpath,int size,const char** fitfilepaths);
void DrawPlot(const char* inputdirectoryname, const char* configfilename  );
#include"MultifitGen/MultifitGen.h"
#include"Reporter/Report.h"
#include"LLRtest/LLRtest.h"
#include"SuperMultifit/supermultifit.h"
#include"ExtraAnalysis/ExtraAnalysis.h"

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
    else if (option=="-report")
    {
         //enter in multicomparation draw mode
        int numFiles=(argc -4) ;
        const char* args[64];

        if (numFiles<=0) return 0;
        
        for (int i=0;i<numFiles;i++) args[i]=argv[i+4];

        Report(argv[2],argv[3],numFiles,args);
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
    else if(option=="-llr")
    {
        int numFiles=(argc -3) ;
        const char* args[64];

        if (numFiles<=0) return 0;
        
        for (int i=0;i<numFiles;i++) args[i]=argv[i+3];
        LLRtest( args, argv[2],numFiles );
    }
    else if(option=="-smult")
    {
        int numFiles=(argc -3) ;
        const char* args[64];

        if (numFiles<=0) return 0;
        
        for (int i=0;i<numFiles;i++) args[i]=argv[i+3];
        Supermultifit( args, argv[2],numFiles );
    }else if(option=="-extraan")
    {
        ExtraAnalysis(argv[2]);
    } else
    {
        std::cerr << "Incorrect option flags\n";  
    }

        
    return 0;
}

#endif