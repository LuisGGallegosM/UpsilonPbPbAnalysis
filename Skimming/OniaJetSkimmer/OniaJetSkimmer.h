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
    using Reader= OniaJetReaderMC;
    Reader oniaReader;
    OniaJetCutterBase<Reader> oniaCutter;
    //OniaWriterJet<Reader> oniaWriter;
    OniaWriterJet oniaWriter;

    JetCorrector JEC;
    JetUncertainty JEU;

    void ProcessEvent(Long64_t entry) override
    {
        Long64_t size=oniaReader.recoQQ.size;
        
        for(Long64_t iQQ=0;iQQ<size;iQQ++)
        {
            if (oniaCutter.cut(&oniaReader,iQQ,entry))
            {
                int iJet=FindJet(&oniaReader.jetInfo,&JEC,(TLorentzVector*)oniaReader.recoQQ.mom4->At(iQQ),&oniaReader.jetRef);
                if(iJet>=0)
                {
                    oniaWriter.write(&oniaReader,iQQ,iJet,entry,&JEC,&JEU);
                }
            }
        }
    }

    public:
    OniaJetSkimmer(TTree* tree , const CutParams* cutter, const char* outTreeName): 
        oniaCutter(cutter) ,oniaReader(tree), oniaWriter(outTreeName),
        JEC(LoadJECFiles(cutter->getIsMC())), JEU(LoadJEUFiles())
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