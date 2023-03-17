
#include "FileHelpers.h"
#include <fstream>

const std::string mainfilepath="../JECDatabase/textFiles";

std::string RemoveFilename(const std::string& filename)
{
    return filename.substr(0,filename.find_last_of('/'));
}

std::string RemoveExtension(const std::string& filename)
{;
    return filename.substr(0,filename.find_last_of('.'));
}

std::string ReplaceExtension(const char* outfilename,const char* newextension)
{
    return RemoveExtension(outfilename) + newextension;
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

TTree* GetTree(TFile* file,const char* name)
{
    TTree* tree =(TTree *)file->Get(name);
    if (tree == nullptr)
    {
        throw std::invalid_argument(std::string(name) + " tree not found\n");
    }
    return tree;
}

std::string getBasename(const std::string& dir)
{
    return dir.substr(dir.find_last_of('/')+1);
}

std::string generateNames(const std::string& dir, const std::string& extension)
{
    return dir + "/" + getBasename(dir) + extension;
}

std::vector<std::string> LoadJECFiles(bool isMC)
{
    std::vector<std::string> jecFilenames;
    if(isMC)
    {
        jecFilenames.push_back(mainfilepath+"/Spring18_ppRef5TeV_V4_MC/Spring18_ppRef5TeV_V4_MC_L2Relative_AK4PF.txt");
     }
    else
    {
        jecFilenames.push_back(mainfilepath+"/Spring18_ppRef5TeV_V4_DATA/Spring18_ppRef5TeV_V4_DATA_L2Relative_AK4PF.txt");
        jecFilenames.push_back(mainfilepath+"/Spring18_ppRef5TeV_V4_DATA/Spring18_ppRef5TeV_V4_DATA_L2L3Residual_AK4PF.txt");
    }

    return jecFilenames;
}

std::string LoadJEUFiles(bool isMC)
{
    std::string jeuFilename;
    if (isMC)
    {
        jeuFilename=mainfilepath+"/Spring18_ppRef5TeV_V4_MC/Spring18_ppRef5TeV_V4_MC_Uncertainty_AK4PF.txt";
    }
    else
    {
        jeuFilename=mainfilepath+"/Spring18_ppRef5TeV_V4_DATA/Spring18_ppRef5TeV_V4_DATA_Uncertainty_AK4PF.txt";
    }
    
    return jeuFilename;
}