
#include "utils.h"

std::string ReplaceExtension(const char* outfilename,const char* newextension)
{
    std::string outputfilenamestr=std::string(outfilename);
    std::string newextname = outputfilenamestr.substr(0,outputfilenamestr.find_last_of('.'));
    return newextname + newextension;
}