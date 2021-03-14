#ifndef ONIAJETSKIMMER
#define ONIAJETSKIMMER

#include "../../TreeProcessor/TreeProcessor.h"
#include "../../OniaBase/OniaIO/OniaIO.h"
#include "../../OniaBase/JEC/JetCorrector.h"
#include "../../OniaBase/JEC/JetUncertainty.h"
#include "../Skimmer.h"
#include "OniaJetCutter.h"

std::vector<std::string> LoadJECFiles(bool isMC);
std::string LoadJEUFiles();
int FindJet(const OniaJetInfo* jetInfo, JetCorrector* JEC, const TLorentzVector* RecoQQ4mom, const OniaJetRef* jetRef=nullptr);

//template<typename Reader>
class OniaJetSkimmer : public TreeProcessor, public Skimmer
{
    private:
    using Reader= OniaJetMCData;
    OniaReader<Reader> oniaReader;
    OniaJetCutterBase<Reader> oniaCutter;
    OniaWriterJetQQ oniaWriter;
    JetCorrector JEC;
    JetUncertainty JEU;

    void ProcessEvent(Long64_t entry) override
    {
        const OniaJetMCData* input = oniaReader.getData();
        Long64_t size=input->recoQQ.size;
        
        for(Long64_t iQQ=0;iQQ<size;iQQ++)
        {
            if (oniaCutter.cut(input,iQQ,entry))
            {
                int iJet=FindJet(&input->jetInfo,&JEC,(TLorentzVector*)input->recoQQ.mom4->At(iQQ),&input->jetRef);
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
        oniaCutter(cutter),JEC(LoadJECFiles(cutter->getIsMC())), JEU(LoadJEUFiles())
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

using OniaJetSkimmerMC = OniaJetSkimmer;
//using OniaJetSkimmerRealData = OniaJetSkimmer<OniaJetRealData>;

std::unique_ptr<Skimmer> createJetSkimmer(TTree* tree ,const CutParams* cutter, const char* outTreeName);


#endif