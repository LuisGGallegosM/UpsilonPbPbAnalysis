

#include"OniaWriter.h"
#include"TLorentzVector.h"

//OniaWriterBase

OniaWriterBase::OniaWriterBase(const char* treeName, QQtype type):
    TreeWriter(treeName), qqType(type)
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
    TLorentzVector* dimuon =(TLorentzVector*) dataIn->genQQ.mom4->At(index);
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
    TLorentzVector* dimuon =(TLorentzVector*) dataIn->recoQQ.mom4->At(index);
    mass= dimuon->M();
    pT = dimuon->Pt();
    y = fabs(dimuon->Rapidity());
    phi = dimuon->Phi();
    eta = dimuon->Eta();
    Evt = entry;
    pdgId=0;
}

void OniaWriterBase::writeEntries(const OniaReader* dataIn, int index, int entry)
{ 
    if (getType()==QQtype::Gen)
        readGenQQ(dataIn,index,entry);
    else
        readRecoQQ(dataIn,index,entry);
    TreeWriter::FillEntries(); 
};

//OniaWriterFull

OniaWriterFull::OniaWriterFull(const char* treeName, QQtype type):
    OniaWriterBase(treeName,type)
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
    TLorentzVector* mom4vec_mumi = (TLorentzVector*) dataIn->recoMu.mom4->At(dataIn->recoQQ.mumi_idx[index]);
    TLorentzVector* mom4vec_mupl = (TLorentzVector*) dataIn->recoMu.mom4->At(dataIn->recoQQ.mupl_idx[index]);
    pT_mi = mom4vec_mumi->Pt();
    eta_mi = mom4vec_mumi->Eta();
    phi_mi = mom4vec_mumi->Phi();

    pT_pl = mom4vec_mupl->Pt();
    eta_pl = mom4vec_mupl->Eta();
    phi_pl = mom4vec_mupl->Phi();
}

void OniaWriterFull::readGenMu(const OniaReader* dataIn, int index, int entry)
{
    TLorentzVector* mom4vec_mumi = (TLorentzVector*) dataIn->genMu.mom4->At(dataIn->genQQ.mumi_idx[index]);
    TLorentzVector* mom4vec_mupl = (TLorentzVector*) dataIn->genMu.mom4->At(dataIn->genQQ.mupl_idx[index]);
    pT_mi = mom4vec_mumi->Pt();
    eta_mi = mom4vec_mumi->Eta();
    phi_mi = mom4vec_mumi->Phi();

    pT_pl = mom4vec_mupl->Pt();
    eta_pl = mom4vec_mupl->Eta();
    phi_pl = mom4vec_mupl->Phi();
}

void OniaWriterFull::writeEntries(const OniaReader* dataIn, int index, int entry)
{ 
    if (getType()==QQtype::Gen)
    {
        readGenQQ(dataIn,index,entry);
        readGenMu(dataIn,index,entry);
    }
    else
    {
        readRecoQQ(dataIn,index,entry);
        readRecoMu(dataIn,index,entry);
    }
    TreeWriter::FillEntries(); 
};