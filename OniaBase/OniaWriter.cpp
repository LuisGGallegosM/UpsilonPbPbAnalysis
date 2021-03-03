

#include"OniaWriter.h"
#include"TLorentzVector.h"

//OniaWriterBase

OniaWriterBase::OniaWriterBase(const char* treeName):
    TreeWriter(treeName)
{
    addOutput("mass",&mass);
    addOutput("eta",&eta);
    addOutput("phi",&phi);
    addOutput("pT",&pT);
    addOutput("y",&y);
    addOutput("Evt",&Evt);
    addOutput("pdfId",&pdgId);
}

void OniaWriterBase::readGenQQ(const OniaReader* dataIn, int index, int entry)
{
    TLorentzVector* dimuon =(TLorentzVector*) dataIn->genQQ_mom4->At(index);
    mass= dimuon->M();
    pT = dimuon->Pt();
    y = fabs(dimuon->Rapidity());
    phi = dimuon->Phi();
    eta = dimuon->Eta();
    Evt = entry;
    pdgId = dataIn->genQQid[index];
}

void OniaWriterBase::readRecoQQ(const OniaReader* dataIn, int index, int entry)
{
    TLorentzVector* dimuon =(TLorentzVector*) dataIn->recoQQ_mom4->At(index);
    mass= dimuon->M();
    pT = dimuon->Pt();
    y = fabs(dimuon->Rapidity());
    phi = dimuon->Phi();
    eta = dimuon->Eta();
    Evt = entry;
    pdgId=0;
}

void OniaWriterBase::writeRecoQQ(const OniaReader* dataIn, int index, int entry)
{
    readRecoQQ(dataIn,index,entry);
    FillEntries(); 
}

void OniaWriterBase::writeGenQQ(const OniaReader* dataIn, int index, int entry)
{
    readGenQQ(dataIn,index,entry);
    FillEntries(); 
}

//OniaWriterFull

OniaWriterFull::OniaWriterFull(const char* treeName):
    OniaWriterBase(treeName)
{
    addOutput("pT_mi",&pT_mi);
    addOutput("pT_pl",&pT_pl);
    addOutput("eta_mi",&eta_mi);
    addOutput("eta_pl",&eta_pl);
    addOutput("phi_mi",&phi_mi);
    addOutput("phi_pl",&phi_pl);
}

void OniaWriterFull::readRecoMu(const OniaReader* dataIn, int index, int entry)
{
    TLorentzVector* mom4vec_mumi = (TLorentzVector*) dataIn->recoMu_mom4->At(dataIn->recoQQ_mumi_idx[index]);
    TLorentzVector* mom4vec_mupl = (TLorentzVector*) dataIn->recoMu_mom4->At(dataIn->recoQQ_mupl_idx[index]);
    pT_mi = mom4vec_mumi->Pt();
    eta_mi = mom4vec_mumi->Eta();
    phi_mi = mom4vec_mumi->Phi();

    pT_pl = mom4vec_mupl->Pt();
    eta_pl = mom4vec_mupl->Eta();
    phi_pl = mom4vec_mupl->Phi();
}

void OniaWriterFull::readGenMu(const OniaReader* dataIn, int index, int entry)
{
    TLorentzVector* mom4vec_mumi = (TLorentzVector*) dataIn->genMu_mom4->At(dataIn->genQQ_mumi_idx[index]);
    TLorentzVector* mom4vec_mupl = (TLorentzVector*) dataIn->genMu_mom4->At(dataIn->genQQ_mupl_idx[index]);
    pT_mi = mom4vec_mumi->Pt();
    eta_mi = mom4vec_mumi->Eta();
    phi_mi = mom4vec_mumi->Phi();

    pT_pl = mom4vec_mupl->Pt();
    eta_pl = mom4vec_mupl->Eta();
    phi_pl = mom4vec_mupl->Phi();
}

void OniaWriterFull::writeRecoQQ(const OniaReader* dataIn, int index, int entry)
{
    readRecoQQ(dataIn,index,entry);
    readRecoMu(dataIn,index,entry);
    FillEntries(); 
}
void OniaWriterFull::writeGenQQ(const OniaReader* dataIn, int index, int entry)
{
    readGenQQ(dataIn,index,entry);
    readGenMu(dataIn,index,entry);
    FillEntries(); 
}