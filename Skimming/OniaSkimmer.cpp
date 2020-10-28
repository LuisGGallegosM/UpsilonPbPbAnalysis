

#include "OniaSkimmer.h"

OniaSkimmer::OniaSkimmer(TTree* treeIn,const char* treeOutName) 
: Skimmer(treeIn,treeOutName)
{
    TBranch* branch;

    //input branches
    addInput("Reco_QQ_4mom",&dataIn.mom4_QQ);
    addInput("Reco_mu_4mom",&dataIn.mom4_mu);
    addInput("Reco_QQ_size",&dataIn.size);
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
    addInput("Reco_mu_whichGen",dataIn.whichGen);

    //output branches
    addOutput("mass",&dataOut.mass);
    addOutput("eta",&dataOut.eta);
    addOutput("phi",&dataOut.phi);
    addOutput("pT",&dataOut.pT);
    addOutput("y",&dataOut.y);
    addOutput("Evt",&dataOut.Evt);
    addOutput("pT_mi",&dataOut.pT_mi);
    addOutput("pT_pl",&dataOut.pT_pl);
    addOutput("eta_mi",&dataOut.eta_mi);
    addOutput("eta_pl",&dataOut.eta_pl);
    addOutput("phi_mi",&dataOut.phi_mi);
    addOutput("phi_pl",&dataOut.phi_pl);

    auxData.reset(new Onia_Aux());
    auxData->events.reserve(200000);

    return;
}

void OniaSkimmer::WriteData(Int_t index, Long64_t entry)
{
    TLorentzVector* mom4vec=(TLorentzVector*) dataIn.mom4_QQ->At(index);
    TLorentzVector* mom4vec_mumi = (TLorentzVector*) dataIn.mom4_mu->At(dataIn.mumi_idx[index]);
    TLorentzVector* mom4vec_mupl = (TLorentzVector*) dataIn.mom4_mu->At(dataIn.mupl_idx[index]);
    dataOut.mass= mom4vec->M();
    dataOut.pT = mom4vec->Pt();
    dataOut.y = mom4vec->Rapidity();
    dataOut.phi = mom4vec->Phi();
    dataOut.eta = mom4vec->Eta();
    dataOut.Evt = entry;

    dataOut.pT_mi = mom4vec_mumi->Pt();
    dataOut.eta_mi = mom4vec_mumi->Eta();
    dataOut.phi_mi = mom4vec_mumi->Phi();

    dataOut.pT_pl = mom4vec_mupl->Pt();
    dataOut.eta_pl = mom4vec_mupl->Eta();
    dataOut.phi_pl = mom4vec_mupl->Phi();

    auxData->events.insert({entry,dataOut});
}

//***********************************************
//Onia structs

Int_t Onia_Input::getSize()
{
    return size;
}

Onia_Input::Onia_Input()
{
    mom4_QQ = new TClonesArray("TLorentzVector");
    mom4_mu = new TClonesArray("TLorentzVector");
    mupl_idx = new Int_t[maxBranchSize];
    mumi_idx = new Int_t[maxBranchSize];
    SelectionType = new Int_t[maxBranchSize];
    nTrkWMea = new Int_t[maxBranchSize];
    nPixWMea = new Int_t[maxBranchSize];
    dxy = new Float_t[maxBranchSize];
    dz = new Float_t[maxBranchSize]; 
    VtxProb = new Float_t[maxBranchSize];
    trig = new ULong64_t[maxBranchSize];
    whichGen = new Int_t[maxBranchSize];
    sign = new Int_t[maxBranchSize];
}

Onia_Input::~Onia_Input()
{
    delete mom4_QQ;
    delete mom4_mu;
    delete[] mupl_idx;
    delete[] mumi_idx;
    delete[] SelectionType;
    delete[] nTrkWMea;
    delete[] nPixWMea;
    delete[] dxy;
    delete[] dz;
    delete[] VtxProb;
    delete[] trig;
    delete[] whichGen;
    delete[] sign;
}