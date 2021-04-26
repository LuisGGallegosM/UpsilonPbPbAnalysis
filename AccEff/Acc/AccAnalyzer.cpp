
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

//AccHistografer

AccHistografer::AccHistografer()
{
    //initialize Histograms
    etaVsPtQQGen = createTH2QQ("EtaPtQQ_Gen",  "p^{#mu#mu}_{t} vs |y^{#mu#mu}|");
    etaVsPtQQDet = createTH2QQ("EtaPtQQ_Det", "p^{#mu#mu}_{t} vs |y^{#mu#mu}| QQ Detected");

    etaVsPtMuGen = createTH2Mu("EtaPtMu_Gen","p^{#mu}_{t} vs |#eta^{#mu}| Mu Generated");
    etaVsPtMuDet = createTH2Mu("EtaPtMu_Det","p^{#mu}_{t} vs |#eta^{#mu}| Mu Detected");

    ptHistQQGen= createTH1(accDenName ,"p^{#mu#mu}_{t} QQ Generated");
    ptHistQQDet= createTH1(accNumName ,"p^{#mu#mu}_{t} QQ Detectable");
}

void AccHistografer::FillGen(const inputs* in,float weight)
{
    ptHistQQGen->Fill(in->pT,weight);
    etaVsPtQQGen->Fill(in->y,in->pT,weight);
    etaVsPtMuGen->Fill(in->etaMuPl,in->ptMuPl,weight);
    etaVsPtMuGen->Fill(in->etaMuMi,in->ptMuMi,weight);
}

void AccHistografer::FillDet(const inputs* in,float weight)
{
    ptHistQQDet->Fill(in->pT,weight);
    etaVsPtQQDet->Fill(in->y,in->pT,weight);
    etaVsPtMuDet->Fill(in->etaMuPl,in->ptMuPl,weight);
    etaVsPtMuDet->Fill(in->etaMuMi,in->ptMuMi,weight);
}

void AccHistografer::Write(const std::string& basename)
{
    //calculate acceptancy
    ptQQAcceptancy=createTEff(ptHistQQDet,ptHistQQGen,"PtQQ_Acceptancy", "Acceptancy;p^{#mu#mu}_{T} ( GeV/c );#alpha");
    etaVsPtQQAcceptancy=createTEff(etaVsPtQQDet,etaVsPtQQGen,"EtaPtQQ_Acceptancy","Acceptancy;|y^{#mu#mu}|;p^{#mu#mu}_{T} ( GeV/c )");

    //write 2D plots
    writeToCanvas(etaVsPtQQGen,    "|y^{#mu#mu}|","p^{#mu#mu}_{T} ( GeV/c )",basename);
    writeToCanvas(etaVsPtQQDet,    "|y^{#mu#mu}|","p^{#mu#mu}_{T} ( GeV/c )",basename);
    writeToCanvas(etaVsPtMuGen,    "|#eta^{#mu}|"   ,"p^{#mu}_{T} ( GeV/c )",basename);
    writeToCanvas(etaVsPtMuDet,    "|#eta^{#mu}|"   ,"p^{#mu}_{T} ( GeV/c )",basename);
    
    etaVsPtQQGen->Write(0,TObject::kOverwrite);
    etaVsPtQQDet->Write(0,TObject::kOverwrite);
    etaVsPtMuGen->Write(0,TObject::kOverwrite);
    etaVsPtMuDet->Write(0,TObject::kOverwrite);

    //write 1D plots
    writeToCanvas(ptHistQQGen,       "p^{#mu#mu}_{T} ( GeV/c )", "N_{Gen}^{#mu#mu}",basename);
    writeToCanvas(ptHistQQDet,       "p^{#mu#mu}_{T} ( GeV/c )", "N_{Det}^{#mu#mu}",basename);
    writeToCanvasEff(ptQQAcceptancy.get(), "p^{#mu#mu}_{T} ( GeV/c )", "#alpha",    basename);

    writeToCanvasEff2D(etaVsPtQQAcceptancy.get(), "|y^{#mu#mu}|" , "p^{#mu#mu}_{T} ( GeV/c )",  basename);

    ptHistQQGen->Write(0,TObject::kOverwrite);
    ptHistQQDet->Write(0,TObject::kOverwrite);
    ptQQAcceptancy->Write(0,TObject::kOverwrite);
    etaVsPtQQAcceptancy->Write(0,TObject::kOverwrite);
}