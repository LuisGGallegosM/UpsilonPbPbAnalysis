
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
        return std::unique_ptr<Skimmer>(new OniaJetSkimmerRealData(tree,cutter,outTreeName));
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

int FindJet(const OniaJetMCData* input, JetCorrector* JEC, int iQQ)
{
    auto jetInfo = &(input->jetInfo);
    const TLorentzVector* RecoQQ4mom= (TLorentzVector*) input->recoQQ.mom4->At(iQQ);
    const int jetNumber= jetInfo->nref;
    const float drmin=0.5f;
    int result=-1;
    for(int iJet=0;iJet< jetNumber;iJet++)
    {
        if (input->jetRef.refpt[iJet] < 0.0f) continue;
        float jt_eta=jetInfo->eta[iJet];
        float jt_phi=jetInfo->phi[iJet];
        float jt_pt_noZJEC = getCorrectedPt(JEC,jetInfo,iJet);
        float z = RecoQQ4mom->Pt()/jt_pt_noZJEC;

        if ((z<0.0f) || (z>1.0f)) continue;

        TLorentzVector v_jet;
        v_jet.SetPtEtaPhiM(jt_pt_noZJEC, jt_eta, jt_phi, jetInfo->m[iJet]);
        if (RecoQQ4mom->DeltaR(v_jet)<=drmin)
        {
            result=iJet;
            break;
        }
    }
    return result;
}

int FindJet(const OniaJetRealData* input, JetCorrector* JEC, int iQQ)
{
    auto jetInfo = &(input->jetInfo);
    const TLorentzVector* RecoQQ4mom= (TLorentzVector*) input->recoQQ.mom4->At(iQQ);
    const int jetNumber= jetInfo->nref;
    const float drmin=0.5f;
    int result=-1;
    for(int iJet=0;iJet< jetNumber;iJet++)
    {
        float jt_eta=jetInfo->eta[iJet];
        float jt_phi=jetInfo->phi[iJet];
        float jt_pt_noZJEC = getCorrectedPt(JEC,jetInfo,iJet);
        float z = RecoQQ4mom->Pt()/jt_pt_noZJEC;

        if ((z<0.0f) || (z>1.0f)) continue;

        TLorentzVector v_jet;
        v_jet.SetPtEtaPhiM(jt_pt_noZJEC, jt_eta, jt_phi, jetInfo->m[iJet]);
        if (RecoQQ4mom->DeltaR(v_jet)<=drmin)
        {
            result=iJet;
            break;
        }
    }
    return result;
}