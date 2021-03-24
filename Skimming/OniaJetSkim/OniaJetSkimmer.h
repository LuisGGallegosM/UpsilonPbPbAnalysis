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
class OniaJetSkimmer : public TreeProcessor, public Skimmer
{
    private:
    OniaReader<Reader> oniaReader;
    OniaJetCutterBase<Reader> oniaCutter;
    OniaWriterJet<Writer> oniaWriter;
    JetCorrector JEC;
    JetUncertainty JEU;

    void ProcessEvent(Long64_t entry) override
    {
        auto input = oniaReader.getData();
        Long64_t size=input->recoQQ.size;
        
        for(Long64_t iQQ=0;iQQ<size;iQQ++)
        {
            if (oniaCutter.cut(input,iQQ,entry))
            {
                int iJet=FindJet(input,&JEC,iQQ);
                if(iJet>=0)
                {
                    oniaWriter.writeData(input,iQQ,iJet,entry,&JEC,&JEU);
                    FillEntries();
                }
            }
        }
    }

    public:
    OniaJetSkimmer(TTree* tree , const CutParams* cutter, const char* outTreeName): 
        TreeProcessor(tree,outTreeName),oniaWriter(), oniaReader(),
        oniaCutter(cutter),JEC(LoadJECFiles(cutter->getIsMC())), JEU(LoadJEUFiles(cutter->getIsMC()))
    {
        registerOutputs(&oniaWriter);
        registerInputs(&oniaReader);
    }

    void Write() override 
    { 
        TreeProcessor::write(); 
    }
    
    void Skim() override 
    {
        Process(); 
    }
};

using OniaJetSkimmerMC = OniaJetSkimmer<OniaJetMCData,OniaJetQQMC>;
using OniaJetSkimmerRealData = OniaJetSkimmer<OniaJetRealData,OniaJetQQRealData>;

std::unique_ptr<Skimmer> createJetSkimmer(TTree* tree ,const CutParams* cutter, const char* outTreeName);


#endif