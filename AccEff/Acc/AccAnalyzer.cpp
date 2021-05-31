
#include "AccAnalyzer.h"
#include "TCanvas.h"
#include "TPie.h"
#include <array>

AccAnalyzer::AccAnalyzer(TTree* input,const char* outTreeName, WeightFunc* weights, bool onlyNCorr) : 
    oniaReader(input), weightFunc(weights), onlyNumCorr(onlyNCorr)
{
}

void AccAnalyzer::ProcessEvent(Long64_t entry)
{
    auto input= oniaReader.getData(entry);
    int size=input->genQQ.size;
    
    for(int index=0;index<size;++index)
    {
        //read variables
        TLorentzVector* mom4vec=(TLorentzVector*) input->genQQ.mom4->At(index);

        AccHistografer::inputs data;
        data.pT = mom4vec->Pt();
        data.y = abs(mom4vec->Rapidity());
        if(data.y > yMax) continue;
        if(data.pT > ptMax) continue;
        
       double weight=1.0;
        if(weightFunc!=nullptr)
        {
            weight=weightFunc->getWeight(data.pT);
        }

        TLorentzVector* mom4vecPl=(TLorentzVector*) input->genQQ.mupl_mom4->At(index);
        TLorentzVector* mom4vecMi=(TLorentzVector*) input->genQQ.mumi_mom4->At(index);

        data.etaMuPl=abs(mom4vecPl->Eta());
        data.etaMuMi=abs(mom4vecMi->Eta());
        data.ptMuPl=mom4vecPl->Pt();
        data.ptMuMi=mom4vecMi->Pt();

        if (onlyNumCorr)
            hists.FillGen(&data);
        else
            hists.FillGen(&data,weight);

        if(accCutter.cut(input,index,entry))
        {
            hists.FillDet(&data,weight);
        }
    }
}
