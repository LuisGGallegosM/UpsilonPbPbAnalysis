
#include "utils.h"
#include <fstream>

std::string ReplaceExtension(const char* outfilename,const char* newextension)
{
    std::string outputfilenamestr=std::string(outfilename);
    std::string newextname = outputfilenamestr.substr(0,outputfilenamestr.find_last_of('.'));
    return newextname + newextension;
}

void CopyFile(const char* inputpath, const char* outputpath)
{
    std::ofstream outputfile(outputpath);
    std::ifstream inputfile(inputpath);
    outputfile << inputfile.rdbuf();
    return;
}