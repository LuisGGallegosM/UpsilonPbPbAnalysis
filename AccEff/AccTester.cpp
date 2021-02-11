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

    bool dataIsMC=(treeIn->GetBranch("Reco_mu_whichGen")!=nullptr);
    if (!dataIsMC)
    {
        throw std::invalid_argument("ERROR : data is not MC\n");
    }
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
    addOutput("dR",&dR);

    hist = new TH1F("dR","dR",1000,-0.1,0.1);
}

void AccOutputer::Write()
{
    TreeOutputer::Write(); 
    hist->Write(); 
}

void AccOutputer::WriteData(const AccEffInput& dataIn,Int_t index, Long64_t entry)
{
    TLorentzVector* mom4vecRecoMu=(TLorentzVector*) dataIn.mom4_RecoMu->At(index);
    int genMuIndex= dataIn.RecoMuWhichGen[index];
    TLorentzVector* mom4vecGenMu=(TLorentzVector*) dataIn.mom4_GenMu->At(genMuIndex);

    mass= mom4vecRecoMu->M();
    pT = mom4vecRecoMu->Pt();
    y = mom4vecRecoMu->Rapidity();
    phi = mom4vecRecoMu->Phi();
    eta = mom4vecRecoMu->Eta();
    Evt = entry;
    float dEta = eta - mom4vecGenMu->Eta();
    float dPhi = phi - mom4vecGenMu->Phi();
    if (dPhi > M_PI) dPhi-=M_PI;
    if (dPhi < -M_PI) dPhi+=M_PI;
    dR=sqrt(dEta*dEta+dPhi*dPhi);

    hist->Fill(dR);
    FillEntries();
}