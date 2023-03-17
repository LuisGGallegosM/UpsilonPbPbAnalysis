#ifndef UTILS
#define UTILS

#include <string>

#include "TTree.h"
#include "TFile.h"

std::string RemoveFilename(const std::string& filename);

std::string RemoveExtension(const std::string& filename);

std::string ReplaceExtension(const char* outfilename,const char* newextension);

void CopyFile(const char* inputpath, const char* outputpath);

TFile* OpenFile(const char* filename, const char* option);

TTree* GetTree(TFile* file,const char* name);

std::string getBasename(const std::string& dir);

std::string generateNames(const std::string& dir, const std::string& extension);

std::vector<std::string> LoadJECFiles(bool isMC);
std::string LoadJEUFiles(bool isMC);

#if defined(__CLING__)
#include "../Utils/utils.cpp"
#endif

#endif