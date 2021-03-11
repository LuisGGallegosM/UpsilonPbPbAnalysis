
#include"OniaJetSkimmer.h"
#include "TLorentzVector.h"

const std::string mainfilepath="../JECDatabase/textFiles";

std::unique_ptr<Skimmer> createJetSkimmer(TTree* tree ,const CutParams* cutter, const char* outTreeName)
{
    if (cutter->getIsMC())
    {
        return std::unique_ptr<Skimmer>(new OniaJetSkimmerMC(tree,cutter,outTreeName));
    }
    else
    {
        //return std::unique_ptr<Skimmer>(new OniaJetSkimmerRealData(tree,cutter,outTreeName));
        return nullptr;
    }
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

std::string LoadJEUFiles()
{
    std::string jeuFilename=mainfilepath+"/Spring18_ppRef5TeV_V4_DATA/Spring18_ppRef5TeV_V4_DATA_Uncertainty_AK4PF.txt";
    return jeuFilename;
}