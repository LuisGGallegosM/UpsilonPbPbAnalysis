
#include"AccEffAux.h"
#include<array>
#include"TCanvas.h"

constexpr std::array<double,7> pTBins  { 0.0f,2.0f,4.0f,6.0f,9.0f,12.0f,30.0f};

TH2F* createTH2QQ(const std::string& name,const std::string& title)
{
    const int binyN = 6;
    const float yMax = 3.0f;
    TH2F* result =new TH2F(name.data(),title.data(),binyN,0.0,yMax,pTBins.size()-1,pTBins.data());
    result->SetStats(false);
    result->Sumw2();
    return result;
}

TH2F* createTH2Mu(const std::string& name,const std::string& title)
{
    const int binPtN = 40;
    const float ptMax = 20.0f;
    const int binEtaN = 40;
    const float etaMax = 4.0f;
    TH2F* result =new TH2F(name.data(),title.data(),binEtaN,0.0,etaMax,binPtN,0.0,ptMax);
    result->SetStats(false);
    result->Sumw2();
    return result;
}

TH1F* createTH1(const std::string& name,const std::string& title)
{
    TH1F* result=new TH1F(name.data(),title.data(),pTBins.size()-1,pTBins.data());
    result->SetStats(false);
    result->Sumw2();
    return result;
}

void writeToCanvasBase(TH1* hist,const std::string& xname,const std::string& yname, const std::string& outname, const std::string& option)
{
    TCanvas canvas("Fit_plot","fit",4,45,600,600);
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

void writeToCanvas(TH2* hist,const std::string& xname,const std::string& yname, const std::string& outname)
{
    writeToCanvasBase(hist,xname,yname,outname,"COLZ");
}

void writeToCanvas(TH1* hist,const std::string& xname,const std::string& yname, const std::string& outname)
{
     writeToCanvasBase(hist,xname,yname,outname,"COL");
}

void writeToCanvasEff(TEfficiency* hist,const std::string& xname,const std::string& yname, const std::string& outname)
{
    TCanvas canvas("Fit_plot","fit",4,45,550,520);
    canvas.cd();
    TPad pad("pad","fit", 0.02, 0.02, 0.98, 0.98);
    pad.Draw();
    pad.cd();
    hist->Draw();
    canvas.Write();
    canvas.SaveAs(outname.data());
}