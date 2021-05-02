
#include "AccAnalyzer.h"
#include "TCanvas.h"
#include "TPie.h"
#include <array>

AccAnalyzer::AccAnalyzer(TTree* input,const char* outTreeName, RooAbsReal* weights) : 
    TreeProcessor(input,outTreeName),oniaWriter(), oniaReader(), weightFunc(weights)
{
    registerInputs(&oniaReader);
    registerOutputs(&oniaWriter);
}

void AccAnalyzer::ProcessEvent(Long64_t entry)
{
    int size=oniaReader.getData()->genQQ.size;
    
    for(int i=0;i<size;++i)
    {
        Analyze(i,entry);
    }
}

void AccAnalyzer::Write(const std::string& basename)
{
    TreeProcessor::write();
    hists.Write(basename);
}

void AccAnalyzer::Analyze(Int_t index, Long64_t entry)
{
    const OniaGenOnlyData* input=oniaReader.getData();
    //read variables
    TLorentzVector* mom4vec=(TLorentzVector*) input->genQQ.mom4->At(index);

    AccHistografer::inputs data;
    data.pT = mom4vec->Pt();
    data.y = fabs(mom4vec->Rapidity());
    if(data.y > 2.4f) return;
    
    float weight=1.0f;
    if(weightFunc!=nullptr)
    {
        weightFunc->getVariables()->setRealValue("pt",data.pT);
        weight=weightFunc->getVal();
    }

    TLorentzVector* mom4vecPl=(TLorentzVector*) input->genQQ.mupl_mom4->At(index);
    TLorentzVector* mom4vecMi=(TLorentzVector*) input->genQQ.mumi_mom4->At(index);

    data.etaMuPl=fabs(mom4vecPl->Eta());
    data.etaMuMi=fabs(mom4vecMi->Eta());
    data.ptMuPl=mom4vecPl->Pt();
    data.ptMuMi=mom4vecMi->Pt();

    hists.FillGen(&data,weight);

    if(accCutter.cut(input,index,entry))
    {
        hists.FillDet(&data,weight);
        oniaWriter.writeData(input,SimpleSelector{entry,index});
        FillEntries();
    }
}

