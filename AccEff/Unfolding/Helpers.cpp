
#include "Helpers.h"
#include "TCanvas.h"
#include "TH2.h"

std::string generatePostfix(int i)
{
   return "_jtpt"+ std::to_string(int(jtptbins_low+jtptbins_width*i))+ std::to_string(int(jtptbins_low+jtptbins_width*(i+1)));
}

void DrawHist(TMatrixD* hist,const std::string& name)
{
    TH2D temp(*hist);
    DrawHist(&temp,name);
}

void DrawHist(TH1* hist,const std::string& name)
{
    auto c1 = new TCanvas();
    hist->SetStats(0);
    hist->Sumw2();
    hist->Draw("colz");
    c1->Draw();
    c1->SaveAs( name.data());
}

void DrawHist2(TH1* hist1, TH1* hist2,const std::string& name)
{
    auto c1 = new TCanvas();
    hist1->SetStats(0);
    hist1->Sumw2();
    hist1->SetLineColor(1);
    hist1->Draw("colz");
    
    hist2->SetStats(0);
    hist2->Sumw2();
    hist2->SetLineColor(2);
    hist2->Draw("same");
    c1->Draw();
    c1->SaveAs( name.data());
}
