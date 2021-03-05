
#include "AccAnalyzer.h"
#include "TCanvas.h"
#include "TPie.h"
#include <array>

AccAnalyzer::AccAnalyzer(OniaReader2* input,AccCutter* accCut, OniaWriter* writer) : 
    oniaWriter(writer), accCutter(accCut),oniaReader(input)
{
    //initialize Histograms
    etaVsPtQQGen = createTH2QQ("y vs pt QQ Generated",  "p^{#mu#mu}_{t} vs |y^{#mu#mu}|");
    etaVsPtQQDet = createTH2QQ("y vs pt QQ Detectable", "p^{#mu#mu}_{t} vs |y^{#mu#mu}| QQ Detected");

    etaVsPtMuGen = createTH2Mu("eta vs pt Mu Generated","p^{#mu}_{t} vs |#eta^{#mu}| Mu Generated");
    etaVsPtMuDet = createTH2Mu("eta vs pt Mu Detectable","p^{#mu}_{t} vs |#eta^{#mu}| Mu Detected");

    ptHistQQGen= createTH1(accDenName ,"p^{#mu#mu}_{t} QQ Generated");
    ptHistQQDet= createTH1(accNumName ,"p^{#mu#mu}_{t} QQ Detectable");
}

void AccAnalyzer::ProcessEvent(Long64_t entry)
{
    Long64_t size=oniaReader->genQQ_size;
    
    for(Long64_t i=0;i<size;++i)
    {
        Analyze(i,entry);
    }
}

void AccAnalyzer::Write(const std::string& basename)
{
    oniaWriter->Write();

    //calculate acceptancy
    ptQQAcceptancy=createTEff(ptHistQQDet,ptHistQQGen,"pt QQ Acceptancy");
    etaVsPtQQAcceptancy=createTEff(etaVsPtQQDet,etaVsPtQQGen,"eta vs pt QQ Acceptancy");

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
    writeToCanvasEff(ptQQAcceptancy.get(), "p^{#mu#mu}_{T} ( GeV/c )", "Acc",       basename+"_PtQQ_Acceptancy.pdf");
    writeToCanvasEff2D(etaVsPtQQAcceptancy.get(), "p^{#mu#mu}_{T} ( GeV/c )", "Acc",  basename+"_EtaPtQQ_Acceptancy.pdf");

    ptHistQQGen->Write(0,TObject::kOverwrite);
    ptHistQQDet->Write(0,TObject::kOverwrite);
    ptQQAcceptancy->Write(0,TObject::kOverwrite);
    etaVsPtQQAcceptancy->Write(0,TObject::kOverwrite);
}

void AccAnalyzer::Analyze(Int_t index, Long64_t entry)
{
    //read variables
    TLorentzVector* mom4vec=(TLorentzVector*) oniaReader->genQQ_mom4->At(index);
    TLorentzVector* mom4vecPl=(TLorentzVector*) oniaReader->genQQ_mupl_mom4->At(index);
    TLorentzVector* mom4vecMi=(TLorentzVector*) oniaReader->genQQ_mumi_mom4->At(index);

    float pT = mom4vec->Pt();
    float y = fabs(mom4vec->Rapidity());
    float eta = mom4vec->Eta();

    if(y > 2.4f) return;
    float etaMuPl=fabs(mom4vecPl->Eta());
    float etaMuMi=fabs(mom4vecMi->Eta());
    float ptMuPl=mom4vecPl->Pt();
    float ptMuMi=mom4vecMi->Pt();

    //fill data hist for all onia and muons
    ptHistQQGen->Fill(pT);
    etaVsPtQQGen->Fill(y,pT);
    etaVsPtMuGen->Fill(etaMuPl,ptMuPl);
    etaVsPtMuGen->Fill(etaMuMi,ptMuMi);

    //fill data for onia with acceptancy cuts
    if(accCutter->cut(oniaReader.get(),index,entry))
    {
        ptHistQQDet->Fill(pT);
        etaVsPtQQDet->Fill(y,pT);
        etaVsPtMuDet->Fill(etaMuPl,ptMuPl);
        etaVsPtMuDet->Fill(etaMuMi,ptMuMi);
        oniaWriter->writeGen(oniaReader.get(),index,entry);
    }
}