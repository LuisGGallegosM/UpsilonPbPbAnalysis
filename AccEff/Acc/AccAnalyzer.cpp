
#include "AccAnalyzer.h"
#include "TCanvas.h"
#include "TPie.h"
#include <array>

AccAnalyzer::AccAnalyzer(TTree* input,const char* outTreeName) : 
    TreeProcessor(input,outTreeName),oniaWriter(), oniaReader()
{
    registerInputs(&oniaReader);
    registerOutputs(&oniaWriter);
}

void AccAnalyzer::ProcessEvent(Long64_t entry)
{
    Long64_t size=oniaReader.getData()->genQQ.size;
    
    for(Long64_t i=0;i<size;++i)
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

    TLorentzVector* mom4vecPl=(TLorentzVector*) input->genQQ.mupl_mom4->At(index);
    TLorentzVector* mom4vecMi=(TLorentzVector*) input->genQQ.mumi_mom4->At(index);

    data.etaMuPl=fabs(mom4vecPl->Eta());
    data.etaMuMi=fabs(mom4vecMi->Eta());
    data.ptMuPl=mom4vecPl->Pt();
    data.ptMuMi=mom4vecMi->Pt();

    hists.FillGen(&data);

    if(accCutter.cut(input,index,entry))
    {
        hists.FillDet(&data);
        oniaWriter.writeData(input,index,entry);
        FillEntries();
    }
}

//AccHistografer

AccHistografer::AccHistografer()
{
    //initialize Histograms
    etaVsPtQQGen = createTH2QQ("y vs pt QQ Generated",  "p^{#mu#mu}_{t} vs |y^{#mu#mu}|");
    etaVsPtQQDet = createTH2QQ("y vs pt QQ Detectable", "p^{#mu#mu}_{t} vs |y^{#mu#mu}| QQ Detected");

    etaVsPtMuGen = createTH2Mu("eta vs pt Mu Generated","p^{#mu}_{t} vs |#eta^{#mu}| Mu Generated");
    etaVsPtMuDet = createTH2Mu("eta vs pt Mu Detectable","p^{#mu}_{t} vs |#eta^{#mu}| Mu Detected");

    ptHistQQGen= createTH1(accDenName ,"p^{#mu#mu}_{t} QQ Generated");
    ptHistQQDet= createTH1(accNumName ,"p^{#mu#mu}_{t} QQ Detectable");
}

void AccHistografer::FillGen(const inputs* in)
{
    ptHistQQGen->Fill(in->pT);
    etaVsPtQQGen->Fill(in->y,in->pT);
    etaVsPtMuGen->Fill(in->etaMuPl,in->ptMuPl);
    etaVsPtMuGen->Fill(in->etaMuMi,in->ptMuMi);
}

void AccHistografer::FillDet(const inputs* in)
{
    ptHistQQDet->Fill(in->pT);
    etaVsPtQQDet->Fill(in->y,in->pT);
    etaVsPtMuDet->Fill(in->etaMuPl,in->ptMuPl);
    etaVsPtMuDet->Fill(in->etaMuMi,in->ptMuMi);
}

void AccHistografer::Write(const std::string& basename)
{
    //calculate acceptancy
    ptQQAcceptancy=createTEff(ptHistQQDet,ptHistQQGen,"pt QQ Acceptancy", "Acceptancy;p^{#mu#mu}_{T} ( GeV/c );#alpha");
    etaVsPtQQAcceptancy=createTEff(etaVsPtQQDet,etaVsPtQQGen,"eta vs pt QQ Acceptancy","Acceptancy;|y^{#mu#mu}|;p^{#mu#mu}_{T} ( GeV/c )");

    //write 2D plots
    writeToCanvas(etaVsPtQQGen,    "|y^{#mu#mu}|","p^{#mu#mu}_{T} ( GeV/c )",basename+"_EtaPtQQ_Gen.pdf");
    writeToCanvas(etaVsPtQQDet,    "|y^{#mu#mu}|","p^{#mu#mu}_{T} ( GeV/c )",basename+"_EtaPtQQ_Det.pdf");
    writeToCanvas(etaVsPtMuGen,    "|#eta^{#mu}|"   ,"p^{#mu}_{T} ( GeV/c )",basename+"_EtaPtMu_Gen.pdf");
    writeToCanvas(etaVsPtMuDet,    "|#eta^{#mu}|"   ,"p^{#mu}_{T} ( GeV/c )",basename+"_EtaPtMu_Det.pdf");
    
    etaVsPtQQGen->Write(0,TObject::kOverwrite);
    etaVsPtQQDet->Write(0,TObject::kOverwrite);
    etaVsPtMuGen->Write(0,TObject::kOverwrite);
    etaVsPtMuDet->Write(0,TObject::kOverwrite);

    //write 1D plots
    writeToCanvas(ptHistQQGen,       "p^{#mu#mu}_{T} ( GeV/c )", "N_{Gen}^{#mu#mu}",basename+"_PtQQ_Gen.pdf");
    writeToCanvas(ptHistQQDet,       "p^{#mu#mu}_{T} ( GeV/c )", "N_{Det}^{#mu#mu}",basename+"_PtQQ_Det.pdf");
    writeToCanvasEff(ptQQAcceptancy.get(), "p^{#mu#mu}_{T} ( GeV/c )", "#alpha",       basename+"_PtQQ_Acceptancy.pdf");

    writeToCanvasEff2D(etaVsPtQQAcceptancy.get(), "|y^{#mu#mu}|" , "p^{#mu#mu}_{T} ( GeV/c )",  basename+"_EtaPtQQ_Acceptancy.pdf");

    ptHistQQGen->Write(0,TObject::kOverwrite);
    ptHistQQDet->Write(0,TObject::kOverwrite);
    ptQQAcceptancy->Write(0,TObject::kOverwrite);
    etaVsPtQQAcceptancy->Write(0,TObject::kOverwrite);
}