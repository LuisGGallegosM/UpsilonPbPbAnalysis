

#include"OniaWriter.h"
#include"TCanvas.h"
#include<map>
#include"TPie.h"
#include<sstream>

void writePie(const std::map<int,int>& values, const std::string& name);

int getRecoPdgId(const OniaReader* dataIn, int index)
{
    int pdgId=0;
    int genQQindex=dataIn->RecoQQWhichGen[index];
    if (genQQindex>=0)
        pdgId= dataIn->genQQ_id[genQQindex];
    return pdgId;
}

//OniaSimpleInfo

OniaSimpleInfo::OniaSimpleInfo(TreeWriter* writer)
{
    writer->addOutput("Evt",&Evt);
    writer->addOutput("pdfId",&pdgId);
}

//OniaSimpleQQ

OniaSimpleQQ::OniaSimpleQQ(TreeWriter* writer)
{
    writer->addOutput("mass",&mass);
    writer->addOutput("eta",&eta);
    writer->addOutput("phi",&phi);
    writer->addOutput("pT",&pT);
    writer->addOutput("y",&y);
}

void OniaSimpleQQ::Write(TLorentzVector* dimuon)
{
    mass= dimuon->M();
    pT = dimuon->Pt();
    y = dimuon->Rapidity();
    phi = dimuon->Phi();
    eta = dimuon->Eta();
}

//OniaSimpleMu

void OniaSimpleMu::Write(TLorentzVector* muonPl, TLorentzVector* muonMi)
{
    pT_mi = muonMi->Pt();
    eta_mi = muonMi->Eta();
    phi_mi = muonMi->Phi();
    pT_pl = muonPl->Pt();
    eta_pl = muonPl->Eta();
    phi_pl = muonPl->Phi();
}

OniaSimpleMu::OniaSimpleMu(TreeWriter* writer)
{
    writer->addOutput("pT_mi",&pT_mi);
    writer->addOutput("pT_pl",&pT_pl);
    writer->addOutput("eta_mi",&eta_mi);
    writer->addOutput("eta_pl",&eta_pl);
    writer->addOutput("phi_mi",&phi_mi);
    writer->addOutput("phi_pl",&phi_pl);
}

//OniaWriter

OniaWriter::OniaWriter(const char* treeName):
    TreeWriter(treeName),oniaInfoOut(this), oniaQQOut(this)
{
}

void OniaWriter::writeReco(const OniaReader* dataIn, int index, int entry)
{
    int pdgId=getRecoPdgId(dataIn,index);
    oniaInfoOut.Write(entry, pdgId);
    oniaQQOut.Write((TLorentzVector*) dataIn->recoQQ_mom4->At(index));
    FillEntries(); 
}

void OniaWriter::writeGen(const OniaReader* dataIn, int index, int entry)
{
    oniaInfoOut.Write(entry,dataIn->genQQ_id[index]);
    oniaQQOut.Write((TLorentzVector*) dataIn->genQQ_mom4->At(index));
    FillEntries(); 
}

void OniaWriter::writeGen(const OniaReader2* dataIn, int index, int entry)
{
    oniaInfoOut.Write(entry,dataIn->genQQ_id[index]);
    oniaQQOut.Write((TLorentzVector*) dataIn->genQQ_mom4->At(index));
    FillEntries(); 
}
//OniaWriterFull

OniaWriterFull::OniaWriterFull(const char* treeName):
    TreeWriter(treeName),oniaInfoOut(this),oniaQQOut(this),oniaMuOut(this)
{
}

void OniaWriterFull::writeReco(const OniaReader* dataIn, int index, int entry)
{
    int pdgId=getRecoPdgId(dataIn,index);
    oniaInfoOut.Write(entry,pdgId);
    oniaQQOut.Write((TLorentzVector*) dataIn->recoQQ_mom4->At(index));
    int mumi_idx= dataIn->recoQQ_mumi_idx[index];
    int mupl_idx= dataIn->recoQQ_mupl_idx[index];
    TLorentzVector* mumi = (TLorentzVector*) dataIn->recoMu_mom4->At(mumi_idx);
    TLorentzVector* mupl = (TLorentzVector*) dataIn->recoMu_mom4->At(mupl_idx);
    oniaMuOut.Write(mupl,mumi);
    FillEntries(); 
}
void OniaWriterFull::writeGen(const OniaReader* dataIn, int index, int entry)
{
    oniaInfoOut.Write(entry,dataIn->genQQ_id[index]);
    oniaQQOut.Write((TLorentzVector*) dataIn->genQQ_mom4->At(index));
    int mumi_idx= dataIn->genQQ_mumi_idx[index];
    int mupl_idx= dataIn->genQQ_mupl_idx[index];
    TLorentzVector* mumi = (TLorentzVector*) dataIn->genMu_mom4->At(mumi_idx);
    TLorentzVector* mupl = (TLorentzVector*) dataIn->genMu_mom4->At(mupl_idx);
    oniaMuOut.Write(mupl,mumi);
    FillEntries(); 
}

void OniaWriterFull::writeGen(const OniaReader2* dataIn, int index, int entry)
{
    oniaInfoOut.Write(entry,dataIn->genQQ_id[index]);
    oniaQQOut.Write((TLorentzVector*) dataIn->genQQ_mom4->At(index));
    TLorentzVector* mumi = (TLorentzVector*) dataIn->genQQ_mumi_mom4->At(index);
    TLorentzVector* mupl = (TLorentzVector*) dataIn->genQQ_mupl_mom4->At(index);
    oniaMuOut.Write(mupl,mumi);
    FillEntries(); 
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