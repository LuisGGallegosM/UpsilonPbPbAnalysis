

#include "OniaSkimmer.h"
#include "TLorentzVector.h"

OniaSkimmer::OniaSkimmer(TTree* treeIn,const char* treeOutName, OniaCutter* cut) 
: Skimmer(treeIn,treeOutName), cutter(cut)
{
    TBranch* branch;

    //input branches
    addInput("Reco_QQ_4mom",&dataIn.mom4_QQ);
    addInput("Reco_mu_4mom",&dataIn.mom4_mu);
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
    

    //MC only branch check
    if((treeIn->GetBranch("Reco_mu_whichGen")!=nullptr) && (!cut->isMC()))
    {
        std::cerr << "\nWARNING: isMC = false and this root file looks like MC (contains 'Reco_mu_whichGen' branch)\n";
    }

    if(cut->isMC())
    {
        addInput("Reco_mu_whichGen",dataIn.RecoMuWhichGen);
        addInput("Gen_QQ_size",&dataIn.genQQsize);
        addInput("Gen_mu_size",&dataIn.genMuSize);
        addInput("Gen_QQ_mupl_idx",dataIn.genQQ_mupl_idx);
        addInput("Gen_QQ_mumi_idx",dataIn.genQQ_mumi_idx);
        addInput("Gen_QQ_momId",dataIn.GenQQid);
    }

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

void OniaSkimmer::ProcessEvent(Long64_t entry)
{
    if (cutter->prescale(entry)) return;
    Long64_t size=dataIn.getSizeRecoQQ();
    
    for(Long64_t j=0;j<size;++j)
    {
        if (cutter->cut(&dataIn,j,entry))
        {
            WriteData(j,entry);
            FillEntries();
        }
    }
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