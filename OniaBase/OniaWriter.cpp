

#include"OniaWriter.h"
#include"TCanvas.h"
#include<map>
#include"TPie.h"
#include<sstream>

void writePie(const std::map<int,int>& values, const std::string& name);

int getRecoPdgId(const OniaReaderMC* dataIn, int index)
{
    int pdgId=0;
    int genQQindex=dataIn->which.RecoQQWhichGen[index];
    if (genQQindex>=0)
        pdgId= dataIn->genQQ.id[genQQindex];
    return pdgId;
}

template<typename Reader>
OniaWriterReco<Reader>::OniaWriterReco(const char* treeName):
    writer(treeName)
{
    oniaInfoOut.addOutputs(&writer);
    oniaQQOut.addOutputs(&writer);
    oniaMuOut.addOutputs(&writer);
}

template<typename Reader>
void OniaWriterReco<Reader>::writeQQ(const Reader* dataIn, int index, int entry)
{
    int pdgId=0;
    oniaInfoOut.Write(entry,pdgId);
    oniaQQOut.Write((TLorentzVector*) dataIn->recoQQ.mom4->At(index));
    int mumi_idx= dataIn->recoQQ.mumi_idx[index];
    int mupl_idx= dataIn->recoQQ.mupl_idx[index];
    TLorentzVector* mumi = (TLorentzVector*) dataIn->recoMu.mom4->At(mumi_idx);
    TLorentzVector* mupl = (TLorentzVector*) dataIn->recoMu.mom4->At(mupl_idx);
    oniaMuOut.Write(mupl,mumi);
    writer.FillEntries(); 
}


template<typename Reader>
OniaWriterGen<Reader>::OniaWriterGen(const char* treeName):
    writer(treeName)
{
    oniaInfoOut.addOutputs(&writer);
    oniaQQOut.addOutputs(&writer);
    oniaMuOut.addOutputs(&writer);
}

template<typename Reader>
void OniaWriterGen<Reader>::writeQQ(const Reader* dataIn, int index, int entry)
{
    oniaInfoOut.Write(entry,dataIn->genQQ.id[index]);
    oniaQQOut.Write((TLorentzVector*) dataIn->genQQ.mom4->At(index));
    int mumi_idx= dataIn->genQQ.mumi_idx[index];
    int mupl_idx= dataIn->genQQ.mupl_idx[index];
    TLorentzVector* mumi = (TLorentzVector*) dataIn->genMu.mom4->At(mumi_idx);
    TLorentzVector* mupl = (TLorentzVector*) dataIn->genMu.mom4->At(mupl_idx);
    oniaMuOut.Write(mupl,mumi);
    writer.FillEntries(); 
}

template<>
void OniaWriterGen<OniaReaderGenOnly>::writeQQ(const OniaReaderGenOnly* dataIn, int index, int entry)
{
    oniaInfoOut.Write(entry,dataIn->genQQ.id[index]);
    oniaQQOut.Write((TLorentzVector*) dataIn->genQQ.mom4->At(index));
    TLorentzVector* mumi = (TLorentzVector*) dataIn->genQQ.mumi_mom4->At(index);
    TLorentzVector* mupl = (TLorentzVector*) dataIn->genQQ.mupl_mom4->At(index);
    oniaMuOut.Write(mupl,mumi);
    writer.FillEntries(); 
}


template class OniaWriterReco<OniaReaderMC>;
template class OniaWriterReco<OniaReaderRealData>;
template class OniaWriterGen<OniaReaderMC>;
template class OniaWriterGen<OniaReaderGenOnly>;

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