

#include"OniaWriter.h"
#include"TLorentzVector.h"

#include"TCanvas.h"
#include<map>
#include"TPie.h"
#include<sstream>

void writePie(const std::map<int,int>& values, const std::string& name);

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
    int genQQindex=dataIn->RecoQQWhichGen[index];
    if (genQQindex>=0)
        pdgId= dataIn->genQQid[genQQindex];
    else
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
    int id=getPdgId();
    if (pdgIds.find(id) == pdgIds.end()) 
        pdgIds[id]=0;
    pdgIds[id]= pdgIds[id]+1;
    FillEntries(); 
}
void OniaWriterFull::writeGenQQ(const OniaReader* dataIn, int index, int entry)
{
    readGenQQ(dataIn,index,entry);
    readGenMu(dataIn,index,entry);
    int id=getPdgId();
    if (pdgIds.find(id) == pdgIds.end()) 
        pdgIds[id]=0;
    pdgIds[id]= pdgIds[id]+1;
    FillEntries(); 
}

void OniaWriterFull::Write()
{
    writePie(pdgIds,"pdgIdPie");
    OniaWriterBase::Write();
}

void writePie(const std::map<int,int>& values, const std::string& name)
{
    TCanvas canvas((name + " plot").data(),"fit",4,45,800,600);
    canvas.cd();
    TPad pad("pad","fit", 0.2, 0.2, 0.80, 0.80);
    pad.Draw();
    pad.cd();
    TPie pie(name.data(),name.data(),values.size());
    int entrynum=0;
    for(const auto& value : values)
    {
        pie.SetEntryVal(entrynum,value.second);
        std::stringstream str;
        str << value.first;
        pie.SetEntryLabel(entrynum, str.str().data());
        entrynum++;
    }
    pie.SetTextSize(0.03f);
    pie.Draw("rsc");
    pie.Write();
    canvas.Write();
    canvas.SaveAs("/media/luisg/ALMACEN/rootfiles/test/pdgId.pdf");
}