
#include"BaseCanvas.h"

#include "TCanvas.h"
#include "TGraphAsymmErrors.h"

constexpr std::array<double,7> pTBins  { 0.0f,2.0f,4.0f,6.0f,9.0f,12.0f,30.0f};
constexpr std::array<double,6> EtaBins { 0.0f,0.5f,1.0f,1.5f,2.0f,2.4f};

TH2F* createTH2QQ(const std::string& name,const std::string& title)
{
    TH2F* result =new TH2F(name.data(),title.data(),EtaBins.size()-1,EtaBins.data(),pTBins.size()-1,pTBins.data());
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

void writeToCanvasBase(TH1* hist,const std::string& xname,const std::string& yname, const std::string& outname, const std::string& option, bool yLog)
{
    const std::string canvasName=std::string(hist->GetName())+" plot";
    const std::string outputFilename=outname+"_"+hist->GetName()+".pdf";
    TCanvas canvas(canvasName.data(),canvasName.data(),canvasWidth,canvasHeight);
    canvas.cd();
    TPad pad("pad","fit", padSizes[0], padSizes[1], padSizes[2], padSizes[3]);
    pad.Draw();
    pad.cd();
    if (yLog) pad.SetLogy();
    hist->GetYaxis()->SetTitle(yname.data());
    hist->GetXaxis()->SetTitle(xname.data());
    hist->Draw(option.data());
    canvas.Write();
    canvas.SaveAs(outputFilename.data());
}

void writeToCanvas(std::vector<TH1*>& hists,const std::string& title,const std::string& xname,const std::string& yname, const std::string& outname, bool yLog)
{
    TCanvas canvas((title+"_plot").data(),(title+" plot").data(),canvasWidth,canvasHeight);
    canvas.cd();
    TPad pad("pad","fit", padSizes[0], padSizes[1], padSizes[2], padSizes[3]);
    pad.Draw();
    pad.cd();
    if (yLog) pad.SetLogy();
    for(auto hist : hists)
    {
        hist->GetYaxis()->SetTitle(yname.data());
        hist->GetXaxis()->SetTitle(xname.data());
        hist->Draw("same");
    }
    pad.BuildLegend(0.7,0.8,1.0,1.0);
    canvas.Write();
    canvas.SaveAs(outname.data());
}

void writeToCanvas(TH2* hist,const std::string& xname,const std::string& yname, const std::string& outname)
{
    writeToCanvasBase(hist,xname,yname,outname,"COLZ",false);
}

void writeToCanvas(TH1* hist,const std::string& xname,const std::string& yname, const std::string& outname, bool yLog)
{
     writeToCanvasBase(hist,xname,yname,outname,"COL",yLog);
}

void writeToCanvasEff(TEfficiency* hist,const std::string& xname,const std::string& yname, const std::string& outname)
{
    const std::string canvasName=std::string(hist->GetName())+" plot";
    const std::string outputFilename=outname+"_"+hist->GetName()+".pdf";
    TCanvas canvas(canvasName.data(),canvasName.data(),canvasWidth,canvasHeight);
    canvas.cd();
    TPad pad("pad","fit", padSizes[0], padSizes[1], padSizes[2], padSizes[3]);
    pad.Draw();
    pad.cd();
    hist->Draw();
    canvas.Write();
    canvas.SaveAs(outputFilename.data());
}

void writeToCanvasEff2D(TEfficiency* hist,const std::string& xname,const std::string& yname, const std::string& outname)
{
    const std::string canvasName=std::string(hist->GetName())+" plot";
    const std::string outputFilename=outname+"_"+hist->GetName()+".pdf";
    TCanvas canvas(canvasName.data(),canvasName.data(),canvasWidth,canvasHeight);
    canvas.cd();
    TPad pad("pad","fit", padSizes[0], padSizes[1], padSizes[2], padSizes[3]);
    pad.Draw();
    pad.cd();
    hist->Draw("COLZ");
    canvas.Write();
    canvas.SaveAs(outputFilename.data());
}