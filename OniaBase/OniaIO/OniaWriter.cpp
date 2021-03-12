

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

float getCorrectedPt(JetCorrector* JEC,const OniaJetInfo* inputJet, int iJet)
{
  JEC->SetJetPT(inputJet->rawpt[iJet]);
  JEC->SetJetEta(inputJet->eta[iJet]);
  JEC->SetJetPhi(inputJet->phi[iJet]);
  JEC->SetRho(1);
  JEC->SetJetArea(inputJet->area[iJet]);
  return JEC->GetCorrectedPT();
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