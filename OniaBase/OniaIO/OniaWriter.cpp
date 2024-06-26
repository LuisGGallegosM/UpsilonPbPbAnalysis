

#include"OniaWriter.h"
#include"TCanvas.h"
#include<map>
#include"TPie.h"
#include<sstream>

void writePie(const std::map<int,int>& values, const std::string& name);

int getRecoPdgId(const OniaMCData* dataIn, int index)
{
  int pdgId=0;
  int genQQindex=dataIn->which.RecoQQWhichGen[index];
  if (genQQindex>=0)
      pdgId= dataIn->genQQ.id[genQQindex];
  return pdgId;
}

void addOutputs(OniaQQ* data,TreeWriter* writer, const char* prefix)
{
    data->oniaInfoOut.addOutputs(writer);
    data->oniaQQOut.addOutputs(writer,prefix);
    data->oniaMuOut.addOutputs(writer,prefix);
}

void addOutputs(OniaQQW* data,TreeWriter* writer, const char* prefix)
{
    data->oniaInfoOut.addOutputs(writer);
    data->oniaQQOut.addOutputs(writer,prefix);
    data->oniaMuOut.addOutputs(writer,prefix);
    data->weight.addOutputs(writer);
}

void addOutputs(OniaJetQQRealData* data,TreeWriter* writer, const char* prefix)
{
    data->oniaInfoOut.addOutputs(writer);
    data->recoQQOut.addOutputs(writer,"reco_");
    data->oniaMuOut.addOutputs(writer, "reco_");
    data->jetOut.addOutputs(writer);
}

void addOutputs(OniaJetQQRealDataW* data,TreeWriter* writer, const char* prefix)
{
    data->oniaInfoOut.addOutputs(writer);
    data->recoQQOut.addOutputs(writer,"reco_");
    data->oniaMuOut.addOutputs(writer, "reco_");
    data->jetOut.addOutputs(writer);
    data->weight.addOutputs(writer);
}

void addOutputs(OniaOnlyW* data,TreeWriter* writer, const char* prefix)
{
    data->weight.addOutputs(writer);
}

void addOutputs(OniaJetQQMC* data,TreeWriter* writer, const char* prefix)
{
    data->oniaInfoOut.addOutputs(writer);
    data->recoQQOut.addOutputs(writer,"reco_");
    data->genQQOut.addOutputs(writer,"gen_");
    data->oniaMuOut.addOutputs(writer, "reco_");
    data->jetOut.addOutputs(writer);
    data->refJetOut.addOutputs(writer);
}

void addOutputs(OniaJetQQMCW* data,TreeWriter* writer, const char* prefix)
{
    data->oniaInfoOut.addOutputs(writer);
    data->recoQQOut.addOutputs(writer,"reco_");
    data->genQQOut.addOutputs(writer,"gen_");
    data->oniaMuOut.addOutputs(writer, "reco_");
    data->jetOut.addOutputs(writer);
    data->refJetOut.addOutputs(writer);
    data->weight.addOutputs(writer);
}

void addInputs(OniaQQ* data, TreeReader* reader)
{
  data->oniaInfoOut.addInputs(reader);
  data->oniaMuOut.addInputs(reader,"reco_");
  data->oniaQQOut.addInputs(reader,"reco_");
}

void addInputs(OniaQQW* data, TreeReader* reader)
{
  data->oniaInfoOut.addInputs(reader);
  data->oniaMuOut.addInputs(reader,"reco_");
  data->oniaQQOut.addInputs(reader,"reco_");
  data->weight.addInputs(reader);
}

void addInputs(OniaJetQQMC* data, TreeReader* reader)
{
    data->oniaInfoOut.addInputs(reader);
    data->recoQQOut.addInputs(reader,"reco_");
    data->genQQOut.addInputs(reader,"gen_");
    data->oniaMuOut.addInputs(reader,"reco_");
    data->jetOut.addInputs(reader);
    data->refJetOut.addInputs(reader);
}

void addInputs(OniaJetQQRealData* data, TreeReader* reader)
{
    data->oniaInfoOut.addInputs(reader);
    data->recoQQOut.addInputs(reader,"reco_");
    data->oniaMuOut.addInputs(reader,"reco_");
    data->jetOut.addInputs(reader);
}

void writeGenQQ(const OniaGenOnlyData* input, OniaQQ* output, SimpleSelector sel)
{
  output->oniaInfoOut.Write(sel.entry,input->genQQ.id[sel.index]);
  output->oniaQQOut.Write((TLorentzVector*) input->genQQ.mom4->At(sel.index));
  TLorentzVector* mumi = (TLorentzVector*) input->genQQ.mumi_mom4->At(sel.index);
  TLorentzVector* mupl = (TLorentzVector*) input->genQQ.mupl_mom4->At(sel.index);
  output->oniaMuOut.Write(mupl,mumi);
}

float getCorrectedPt(JetCorrector* JEC,const OniaJetInfo* inputJet, int iJet)
{
  JEC->SetJetPT(inputJet->rawpt[iJet]);
  JEC->SetJetEta(inputJet->eta[iJet]);
  JEC->SetJetPhi(inputJet->phi[iJet]);
  JEC->SetRho(1);
  JEC->SetJetArea(inputJet->area[iJet]);
  return JEC->GetCorrectedPT();
}

float getJettyCorr(JetCorrector* JEC,const OniaJetInfo* inputJet, int iJet,float recoQQpt)
{
  float result = recoQQpt;
  if ( inputJet->rawpt[iJet] > recoQQpt ) 
  {
    JEC->SetJetPT(inputJet->rawpt[iJet]-recoQQpt);
    JEC->SetJetEta(inputJet->eta[iJet]);
    JEC->SetJetPhi(inputJet->phi[iJet]);
    JEC->SetRho(1);
    JEC->SetJetArea(inputJet->area[iJet]);
    result = recoQQpt+JEC->GetCorrectedPT();
  }
  return result;
}

float zTolerance(float z)
{
  float result=z;
  if (z >= 1.0f && z <= 1.0001f) result = 0.9999f;
  return result;
}

double jeuCorr (double jtPt, double z, double jeu) 
{
  return ( (1-z)*(1+jeu)*jtPt + z*jtPt );
}

double jecCorr(double jtPt, double rawPt, double jpsiPt) 
{
  return ( (1-(jpsiPt/rawPt))*jtPt + ((jpsiPt/rawPt)/(jtPt/rawPt))*jtPt );
}

double zjecCorr(double jtPt, double rawPt, double z) 
{
  return ( (1-z)*jtPt + z*rawPt );
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

TH2F* createTH2Z(const std::string& name,const std::string& title)
{
    const int binyN = 10;
    TH2F* result =new TH2F(name.data(),title.data(),binyN,0.0,1.0,binyN,0.0,1.0);
    result->SetStats(false);
    result->Sumw2();
    return result;
}

void writeToCanvasZ(TH1* hist,const std::string& xname,const std::string& yname, const std::string& outname, const std::string& option)
{
    const std::string canvasName=std::string(hist->GetName())+" plot";
    TCanvas canvas(canvasName.data(),canvasName.data(),4,45,600,600);
    canvas.cd();
    TPad pad("pad","fit", 0.08, 0.08, 0.92, 0.92);
    pad.Draw();
    pad.cd();
    hist->GetYaxis()->SetTitle(yname.data());
    hist->GetXaxis()->SetTitle(xname.data());
    hist->Draw(option.data());
    canvas.Write();
    canvas.SaveAs(outname.data());
}