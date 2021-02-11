
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

TFile* OpenFile(const char* filename, const char* option)
{
    TFile* file = TFile::Open(filename, option);
    //input file is found? is output filename valid?
    if (file==nullptr) 
    {
        throw std::runtime_error(std::string("file ") + filename +" cannot be read\n");
    }
    return file;
}