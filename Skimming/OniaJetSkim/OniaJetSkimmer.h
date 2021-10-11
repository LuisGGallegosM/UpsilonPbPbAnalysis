#ifndef ONIAJETSKIMMER
#define ONIAJETSKIMMER

#include "../../OniaBase/TreeProcessor/TreeProcessor.h"
#include "../../OniaBase/OniaIO/OniaIO.h"
#include "../../OniaBase/JetCorrections/JetCorrections.h"
#include "../Common/Skimmer.h"
#include "OniaJetCutter.h"

std::vector<std::string> LoadJECFiles(bool isMC);
std::string LoadJEUFiles(bool isMC);
int FindJet(const OniaJetInfo* jetInfo, JetCorrector* JEC, const TLorentzVector* RecoQQ4mom, const OniaJetRef* jetRef=nullptr);

template<typename Reader,typename Writer>
class OniaJetSkimmer : public Skimmer
{
    private:
    OniaReader<Reader> oniaReader;
    OniaJetCutterBase<Reader> oniaCutter;
    OniaWriter<Writer> oniaWriter;
    JetCorrector JEC;
    JetUncertainty JEU;

    public:
    OniaJetSkimmer(TTree* tree , const CutParams* cutter, const char* outTreeName): 
        oniaWriter(outTreeName,"reco_"), oniaReader(tree),oniaCutter(cutter),
        JEC(LoadJECFiles(cutter->getIsMC())), JEU(LoadJEUFiles(cutter->getIsMC()))
    {
    }

    void Write() override 
    { 
        oniaWriter.getTree()->Write(0,TObject::kOverwrite); 
    }
    
    void Skim() override 
    {
        TreeProcess(this,oniaReader.getName(),oniaReader.getEntries());
    }

    void ProcessEvent(Long64_t entry)
    {
        auto input = oniaReader.getData(entry);
        int size=input->recoQQ.size;
        
        for(int iQQ=0;iQQ<size;iQQ++)
        {
            if (oniaCutter.cut(input,iQQ,entry))
            {
                int iJet=FindJet(input,&JEC,iQQ);
                if(iJet>=0)
                {
                    writeJet(input,oniaWriter.getDataBuffer(),JetSelector{entry,iQQ,iJet,&JEC,&JEU});
                    oniaWriter.writeData();
                }
            }
        }
    }

};

using OniaJetSkimmerMC = OniaJetSkimmer<OniaJetMCData,OniaJetQQMC>;
using OniaJetSkimmerRealData = OniaJetSkimmer<OniaJetRealData,OniaJetQQRealData>;

std::unique_ptr<Skimmer> createJetSkimmer(TTree* tree ,const CutParams* cutter, const char* outTreeName);


#endif