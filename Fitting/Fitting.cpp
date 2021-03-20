

#include <string>
#include <iostream>

void Fit(const char* filename, const char* outfilename, const char* configname);
void DrawCmp( const char* outputfilename, int size,const char** fitfilenames);
void DrawPlot(const char* inputdirectoryname, const char* configfilename  );

#if !defined(__CLING__)

int main(int argc, char **argv)
{
    std::string option(argv[1]);

    if (option=="-multidraw")
    {
        //enter in multicomparation draw mode
        int numFiles=(argc -3) ;
        const char* args[32];

        if (numFiles<=0) return 0;
        
        for (int i=0;i<numFiles;i++) args[i]=argv[i+3];

        DrawCmp(argv[2],numFiles,args);
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
    else
    if (argc ==4)
    {
        Fit(argv[1],argv[2],argv[3]);
    }
    else
    {
        std::cerr << "Incorrect number of parameters\n";  
    }
        
    return 0;
}

#endif