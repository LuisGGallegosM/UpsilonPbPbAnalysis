#ifndef ONIAJETSKIMMER
#define ONIAJETSKIMMER

#include "../../TreeProcessor/TreeProcessor.h"
#include "../../OniaBase/OniaReader.h"
#include "../../OniaBase/OniaWriter.h"
#include "../../OniaBase/JEC/JetCorrector.h"
#include "../../OniaBase/JEC/JetUncertainty.h"
#include "../Skimmer.h"
#include "OniaJetCutter.h"

std::vector<std::string> LoadJECFiles(bool isMC);
std::string LoadJEUFiles();

//template<typename Reader>
class OniaJetSkimmer : public TreeProcessor, public Skimmer
{
    private:
    using Reader= OniaJetReaderMC;
    Reader oniaReader;
    OniaJetCutterBase<Reader> oniaCutter;
    //OniaWriterJet<Reader> oniaWriter;
    OniaWriterJet oniaWriter;

    JetCorrector jecCorrector;
    JetUncertainty jecUncertainty;

    void ProcessEvent(Long64_t entry) override
    {
        Long64_t size=oniaReader.recoQQ.size;
        
        for(Long64_t iQQ=0;iQQ<size;iQQ++)
        {
            if (oniaCutter.cut(&oniaReader,iQQ,entry))
            {
                int iJet=FindJet(iQQ);
                if(iJet< oniaReader.jetInfo.nref)
                {
                    oniaWriter.writeQQ(&oniaReader,iQQ,iJet,entry,&jecCorrector,&jecUncertainty);
                }
            }
        }
    }

    int FindJet(int index)
    {
        const int jetNumber= oniaReader.jetInfo.nref;
        const float drmin=0.5f;
        int iJet=0;
        for(;iJet< jetNumber;iJet++)
        {
            if (oniaReader.jetInfo.refpt[iJet] < 0.0f) continue;
            float jt_eta=oniaReader.jetInfo.eta[iJet];
            float jt_phi=oniaReader.jetInfo.phi[iJet];
            jecCorrector.SetJetPT(oniaReader.jetInfo.rawpt[iJet]);
            jecCorrector.SetJetEta(jt_eta);
            jecCorrector.SetJetPhi(jt_phi);
            jecCorrector.SetRho(1);
            jecCorrector.SetJetArea(oniaReader.jetInfo.area[iJet]);
            float jt_pt_noZJEC = jecCorrector.GetCorrectedPT();

            TLorentzVector v_jet;
	        v_jet.SetPtEtaPhiM(jt_pt_noZJEC, jt_eta, jt_phi, oniaReader.jetInfo.m[iJet]);
            TLorentzVector* RecoQQ4mom= (TLorentzVector*) oniaReader.recoQQ.mom4->At(index);
            if (RecoQQ4mom->DeltaR(v_jet)<=drmin) break;
        }
        return iJet;
    }

    public:
    OniaJetSkimmer(TTree* tree , const CutParams* cutter, const char* outTreeName): 
        oniaCutter(cutter) ,oniaReader(tree), oniaWriter(outTreeName),
        jecCorrector(LoadJECFiles(cutter->getIsMC())), jecUncertainty(LoadJEUFiles())
    {
    }

    void Write() override 
    { 
        oniaWriter.Write(); 
    }
    
    void Skim() override 
    {
        Process(oniaReader.getReader()); 
    }
};

using OniaJetSkimmerMC = OniaJetSkimmer;
//using OniaJetSkimmerRealData = OniaJetSkimmer<OniaJetReaderRealData>;

std::unique_ptr<Skimmer> createJetSkimmer(TTree* tree ,const CutParams* cutter, const char* outTreeName);


#endif