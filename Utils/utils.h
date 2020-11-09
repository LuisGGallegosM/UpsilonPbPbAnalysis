#ifndef UTILS
#define UTILS

#include <string>

std::string ReplaceExtension(const char* outfilename,const char* newextension);

void CopyFile(const char* inputpath, const char* outputpath);

#endif