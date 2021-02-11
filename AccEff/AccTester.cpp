#include "AccTester.h"
#include "TLorentzVector.h"

AccTester::AccTester(TTree* treeIn, AccOutputer* outp , AccEffCutter* cut) 
: TreeProcessor(treeIn), cutter(cut), outputer(outp)
{
    TBranch* branch;

    //input branches
    addInput("Reco_QQ_4mom",&dataIn.mom4_RecoQQ);
    addInput("Reco_mu_4mom",&dataIn.mom4_RecoMu);
    addInput("Reco_QQ_size",&dataIn.recoQQsize);
    addInput("Reco_mu_size",&dataIn.recoMuSize);
    addInput("Reco_QQ_mupl_idx",dataIn.mupl_idx);
    addInput("Reco_QQ_mumi_idx",dataIn.mumi_idx);
    addInput("Reco_mu_SelectionType",dataIn.SelectionType);
    addInput("Reco_mu_nTrkWMea",dataIn.nTrkWMea);
    addInput("Reco_mu_nPixWMea",dataIn.nPixWMea);
    addInput("Reco_mu_dxy",dataIn.dxy);
    addInput("Reco_mu_dz",dataIn.dz);
    addInput("Reco_QQ_VtxProb",dataIn.VtxProb);
    addInput("Reco_QQ_trig",dataIn.trig);
    addInput("Reco_QQ_sign",dataIn.sign);
    addInput("Reco_mu_whichGen",dataIn.RecoMuWhichGen);
    addInput("Gen_QQ_4mom",&dataIn.mom4_GenQQ);
    addInput("Gen_mu_4mom",&dataIn.mom4_GenMu);
    addInput("Gen_QQ_size",&dataIn.genQQsize);
    addInput("Gen_mu_size",&dataIn.genMuSize);
    addInput("Gen_QQ_mupl_idx",dataIn.genQQ_mupl_idx);
    addInput("Gen_QQ_mumi_idx",dataIn.genQQ_mumi_idx);
    addInput("Gen_QQ_momId",dataIn.GenQQid);

    bool dataIsMC=(treeIn->GetBranch("Reco_mu_whichGen")!=nullptr);
    if (!dataIsMC)
    {
        throw std::invalid_argument("ERROR : data is not MC\n");
    }

    std::cout << "Using " << cutter->getName() <<" cutter.\n";
    return;
}

void AccTester::ProcessEvent(Long64_t entry)
{
    if (cutter->prescale(entry)) return;
    Long64_t size=dataIn.getSizeGenMu();
    
    for(Long64_t i=0;i<size;++i)
    {
        if (cutter->cut(&dataIn,i,entry))
        {
            outputer->WriteData(dataIn,i,entry);
        }
    }
}

AccOutputer::AccOutputer(const char* treeOutName) : TreeOutputer(treeOutName)
{
    addOutput("mass",&mass);
    addOutput("eta",&eta);
    addOutput("phi",&phi);
    addOutput("pT",&pT);
    addOutput("y",&y);
    addOutput("Evt",&Evt);

    hist = new TH1F("dR","dR",1000,-0.1,0.1);
}

void AccOutputer::Write()
{
    TreeOutputer::Write(); 
    hist->Write(); 
}

void AccOutputer::WriteData(const AccEffInput& dataIn,Int_t index, Long64_t entry)
{
    TLorentzVector* mom4vec=(TLorentzVector*) dataIn.mom4_GenMu->At(index);

    mass= mom4vec->M();
    pT = mom4vec->Pt();
    y = mom4vec->Rapidity();
    phi = mom4vec->Phi();
    eta = mom4vec->Eta();
    Evt = entry;
    FillEntries();
}